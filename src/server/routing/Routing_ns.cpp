#include "Routing_ns.hpp"

LocationRules Routing::determineResourceLocation(ServerConfig serverConfig, HTTPRequest httpRequest)
{
    std::string uri = httpRequest.getURI();
    std::map<std::string, LocationRules> locations = serverConfig.getLocations();
    while (!uri.empty()) {
        std::map<std::string, LocationRules>::iterator it = locations.find(uri);
        if (it != locations.end())
            return it->second;

        std::size_t pos = uri.find_last_of('/');
        if (pos == std::string::npos || uri == "/")
            break;

        uri = uri.substr(0, pos);
        if (uri.empty())
            uri = "/";// verify if "/" URI doesn' t exist
    }
    return locations["default"];
}

bool Routing::isAllowedMethod(const std::string & method, const std::list<std::string> & allowed_methods)
{
    std::list<std::string>::const_iterator it = std::find(allowed_methods.begin(), 
                                            allowed_methods.end(), 
                                            method);
    return it != allowed_methods.end();
}

std::string Routing::removeKeyValue(std::string toRemove, std::string str)
{
    size_t pos;
    if (toRemove == "default")
        return str;
    pos = str.find(toRemove);
    return str.substr(pos + toRemove.length());
}

short int Routing::typeOfResource(const std::string& path)
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    else if (S_ISREG(statbuf.st_mode))
        return ISFILE; // Verifica si es un archivo regular
    else if (S_ISDIR(statbuf.st_mode))
        return ISDIR;
    return false;
}

Response Routing::processFilePath(std::string resource_path)
{
    Response response;
    std::string buffer;

    std::ifstream inFileStream(resource_path.c_str());
    
    if (inFileStream.is_open())
    {
        std::string		buffer;
		inFileStream.seekg(0, std::ios::end);
		buffer.resize(inFileStream.tellg());
		inFileStream.seekg(0, std::ios::beg);
		inFileStream.read(&buffer[0], buffer.size());
        inFileStream.close();
        response.file_path = resource_path;
        response.response_code = 200;
        response.string_body = buffer;
        return response;
    }
    response.response_code = 403;
    return response;
}

Response Routing::processDirPath(std::string root, std::string resource_path, LocationRules locationRule)
{
    Response response;
    std::string default_file = root + "/"+locationRule.getIndex();
    std::string buffer;

    std::cout << default_file << std::endl;
    if (!default_file.empty() && !access(default_file.c_str(), R_OK))//try to open a default file
        return processFilePath(default_file);
    else if (default_file.empty() && locationRule.isAuto_index()) // try  to open dir
    {
        DIR * dir;
        struct dirent *entry;
        dir = opendir(resource_path.c_str());
        if(dir != NULL)
        {
            while ((entry = readdir(dir)) != NULL)
            {
                response.string_body += entry->d_name; // Agrega el nombre del archivo/directorio a la lista
                response.string_body += "\n"; // Agrega un salto de línea después de cada nombre
            }
            closedir(dir);
            response.file_path = resource_path;
            response.response_code = 200;
            return response;
        }
    }
    response.file_path = "";
    response.response_code = 403;
    return response;
}

bool    Routing::isCorrectCGIExtension(const std::string & file_path, const std::string & extension)
{
    std::size_t lastDotPos = file_path.find_last_of('.');
    if (lastDotPos == std::string::npos)
        return false;
    
    std::string fileExtension = file_path.substr(lastDotPos);
    return fileExtension == extension;
}

void    Routing::errorResponse(Response & response, LocationRules & locationRule)
{
    if (response.response_code > 399) // in RFC 7231, 4xx are client errors
    {
        int response_code = response.response_code;//auxiliary variable to save the response code
        std::map<int, std::string> error_pages = locationRule.getErrorPages();
        std::map<int, std::string>::iterator it = error_pages.find(response.response_code);
        if (it != error_pages.end()) {
            response.file_path = locationRule.getRoot() + it->second;
            response = processFilePath(response.file_path);
            response.response_code = response_code;
        }
        else
            response.file_path = "";
    }
}

Response    Routing::determinePathRequestedResource(HTTPRequest httpRequest, LocationRules locationRule)
{
    std::string file_path;
    std::string body;
    Response response;
    
    if (isAllowedMethod(httpRequest.getMethod(), locationRule.getAllowedMethods()))
    {
        file_path = locationRule.getRoot() + removeKeyValue(locationRule.getKeyValue(), httpRequest.getURI());
        switch(typeOfResource(file_path))
        {
            case ISDIR:
                response = processDirPath(locationRule.getRoot(), file_path, locationRule);//process directory
                break;
            case ISFILE:
                if (!locationRule.getCgiPass().empty() && \
                    isCorrectCGIExtension(file_path, locationRule.getCgiExtension()))
                    std::cout << "es un cgi!!" << std::endl;//toca meterse a ejecutar el cgi
                response = processFilePath(file_path);//process file 
                break;
            default:
                response.response_code = 404;
        }
    }
    else
        response.response_code = 405;
    errorResponse(response, locationRule);
    return response;
}

Response        Routing::returnResource(ServerConfig serverConfig, HTTPRequest httpRequest)
{
    LocationRules location = determineResourceLocation(serverConfig, httpRequest);
    return determinePathRequestedResource(httpRequest, location);
}
