#ifndef ServerConfig_hpp
# define ServerConfig_hpp

# include <iostream>
# include <map>
# include <vector>
# include "LocationRules.hpp"
# include <fstream>
# include <string>
# include <cstdlib>

class ServerConfig
{
private:
    int port;
    std::vector<std::string> serverName;
    std::string root;
    std::map<std::string, std::string> errorPages;
    int maxBodySize;
    std::map<std::string, LocationRules> locations; //Mapa donde almacenamos la info de las locations

public:
    int getPort() const;
    std::vector<std::string> getServerName();
    ServerConfig(std::ifstream& myFile, int position);
    ~ServerConfig();
};

#endif
