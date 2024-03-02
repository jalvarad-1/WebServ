#include "Routing_ns.hpp"

LocationRules & Routing::determineResourceLocation(ServerConfig & serverConfig, HTTPRequest httpRequest)
{
    std::string uri = httpRequest.getURI();
    while (!uri.empty()) {
        std::map<std::string, LocationRules>::iterator it = serverConfig.locations.find(uri);
        if (it != serverConfig.locations.end())
            return it->second;

        std::size_t pos = uri.find_last_of('/');
        if (pos == std::string::npos || uri == "/")
            break;

        uri = uri.substr(0, pos);
        if (uri.empty())
            uri = "/";
    }
    return serverConfig.locations["default"];
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

short int Routing::typeOfResource(const std::string& path, LocationRules locationRule, std::string method)
{
    struct stat statbuf;

    if ((!locationRule.getCgiPass().empty() && \
                    isCorrectCGIExtension(path, locationRule.getCgiExtension())) || \
					method == "DELETE") {
        return ISCGI;
    }
    if (stat(path.c_str(), &statbuf) != 0)
        return ISNF;
    else if (method != "GET")
        return ISNAM; 
    else if (S_ISREG(statbuf.st_mode))
        return ISFILE;
    else if (S_ISDIR(statbuf.st_mode))
        return ISDIR;
    return ISNAM;
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

Response Routing::processDirPath(std::string resource_path, LocationRules locationRule)
{
    Response response;
    std::string default_file;
    if (!locationRule.getIndex().empty())
        default_file = resource_path + "/" + locationRule.getIndex();
    std::string buffer;

    if (!default_file.empty() && !access(default_file.c_str(), R_OK))
        return processFilePath(default_file);
    else if (locationRule.isAuto_index())
    {
        DIR * dir;
        struct dirent *entry;
        dir = opendir(resource_path.c_str());
        if(dir != NULL)
        {
            while ((entry = readdir(dir)) != NULL) {
                response.string_body += entry->d_name;
                response.string_body += "\n";
            }
            closedir(dir);
            response.file_path = resource_path;
            response.response_code = 200;
            return response;
        }
    }
    response.file_path = "";
    response.response_code = 404;
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
        int response_code = response.response_code;
        std::map<int, std::string> error_pages = locationRule.getErrorPages();
        std::map<int, std::string>::iterator it = error_pages.find(response.response_code);
        if (it != error_pages.end()) {
            response.file_path = locationRule.getRoot() + "/" + it->second;
            response = processFilePath(response.file_path);
            response.response_code = response_code;
        }
        else
            response.file_path = "";
    }
}

std::string Routing::createFilePath(LocationRules locationRule, HTTPRequest& httpRequest) {
    std::string uri = httpRequest.getURI();
    std::string cgi_extension = locationRule.getCgiExtension();

    if (!locationRule.getCgiPass().empty()) {
        size_t extension_pos = uri.find(cgi_extension);
        if (extension_pos != std::string::npos) {
            size_t after_extension_pos = extension_pos + cgi_extension.length();
            if (after_extension_pos == uri.length() || uri[after_extension_pos] == '/') {
                httpRequest.setPathInfo(uri.substr(after_extension_pos));
                uri.erase(after_extension_pos);
            }
        }
    }
    std::string final_path = locationRule.getRoot() + removeKeyValue(locationRule.getKeyValue(), uri);
    if (!final_path.empty() && final_path[final_path.size() - 1] == '/') {
        final_path.erase(final_path.size() - 1);
    }
    return final_path;
}
