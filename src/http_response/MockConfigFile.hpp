#ifndef MockConfigFile_hpp
# define MockConfigFile_hpp

#include <iostream>
#include <map>
#include <vector>

class LocationRules
{
    private:
    //Variables de info de la location

    public:
        LocationRules();
        ~LocationRules();
};

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

class ConfigParser
{
    private:
        std::vector<ServerConfig> confServers;
    public:
        ConfigParser(std::string path);
        ~ConfigParser();
};

#endif