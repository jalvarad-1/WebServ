#ifndef MockConfigFile_hpp
# define MockConfigFile_hpp

#include <iostream>
#include <map>
#include <vector>
#include <list>

class LocationRules
{
    private:
        int maxBodySize;
        std::list<std::string> allowedMethods;
        //redirect loquesea.com  -> debe enviar stauts 300 y además un header con Location y el redirect
        //listing directory on or of
        //Set a default file to answer if the request is a directory.
    //Variables de info de la location

    public:
        LocationRules();
        ~LocationRules();
};

class ServerConfig
{
    private:
        int port;
        // int host ; ¿?
        std::list<std::string> serverNames;
        std::string root;
        std::map<std::string, std::string> errorPages;
        int maxBodySize;
        std::map<std::string, LocationRules> locations; //Mapa donde almacenamos la info de las locations

    public:
        ServerConfig();
        ~ServerConfig();
        LocationRules findBestMatchingLocation(const std::string &uri) const
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