#ifndef ServerConfig_hpp
# define ServerConfig_hpp

#include <iostream>
#include <map>
#include <vector>
#include "LocationRules.hpp"

class ServerConfig
{
private:
    int port;
    std::string serverName;
    std::string root;
    std::map<std::string, std::string> errorPages;
    int maxBodySize;
    std::map<std::string, LocationRules> locations; //Mapa donde almacenamos la info de las locations

public:
    ServerConfig();
    ~ServerConfig();
};

#endif
