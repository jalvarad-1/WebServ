#ifndef Routing_hpp
#define Routing_hpp
#include "../../ConfigParser/ServerConfig.hpp"
#include "../../http_request/HTTPRequest.hpp"
#include "../CGI/CGI.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>

#define ISNF 0
#define ISDIR 1
#define ISFILE 2
#define ISCGI 3
#define ISNAM 4

class HTTPRequest;
namespace Routing
{
    LocationRules  & determineResourceLocation(ServerConfig &serverConfig, HTTPRequest httpRequest);
    bool            isAllowedMethod(const std::string & method, const std::list<std::string> & allowed_methods);
    std::string     removeKeyValue(std::string toRemove, std::string str);
    short int       typeOfResource(const std::string& path, LocationRules locationRule, std::string method);

    Response        processDirPath(std::string file_path, LocationRules locationRule);
    Response        processFilePath(std::string resource_path);
    bool            isCorrectCGIExtension(const std::string & file_path, const std::string & extension);
    void            errorResponse(Response & response, LocationRules & locationRule);
    std::string     createFilePath(LocationRules locationRule, HTTPRequest& httpRequest);
}

#endif
