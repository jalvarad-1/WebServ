#ifndef Routing_hpp
#define Routing_hpp
#include "../../ConfigParser/ServerConfig.hpp"
#include "../../http_request/HTTPRequest.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>


#define ISDIR 1
#define ISFILE 2

struct Response {
    std::string     string_body;
    std::string     file_path;
    int             response_code;
};

namespace Routing
{
    Response        returnResource(ServerConfig serverConfig, HTTPRequest httpRequest);//0
    LocationRules   determineResourceLocation(ServerConfig serverConfig, HTTPRequest httpRequest);//1
    Response        determinePathRequestedResource(HTTPRequest httpRequest, LocationRules locationRule);//2
    bool            isAllowedMethod(const std::string & method, const std::list<std::string> & allowed_methods);//3
    std::string     removeKeyValue(std::string toRemove, std::string str);//3
    short int       typeOfResource(const std::string& path);//3

    Response        processDirPath(std::string root, std::string file_path, LocationRules locationRule);//3
    Response        processFilePath(std::string resource_path);//3
}
/*
    
    * función que reciba el location y devuelva el path necesario,
    comprobando ya si es posible abrir y si existe
        -  path archivo, path archivo a mostrar en caso de error, array con códigos [cgi?, directorio?]?,

    * en caso de ser un resultado del cgi, hay que parsear sus headers y devolverlos
    https://www.oreilly.com/openbook/cgi/ch03_02.html
*/

#endif