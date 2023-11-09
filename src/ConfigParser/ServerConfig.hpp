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
    int                                     _port;
    std::vector<std::string>                _serverName;
    std::string                             _root;
    std::map<std::string, std::string>      _errorPages;
    int                                     _maxBodySize;
    std::map<std::string, LocationRules>    _locations; //Mapa donde almacenamos la info de las locations

public:
    ServerConfig();
    ~ServerConfig();
    void setServerName(std::string serverName);
    std::vector<std::string> getServerName();
    void setPort(unsigned int port);
    int getPort();
};

#endif
