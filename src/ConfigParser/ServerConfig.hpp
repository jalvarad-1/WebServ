#ifndef ServerConfig_hpp
# define ServerConfig_hpp

# include <iostream>
# include <map>
# include <vector>
# include <list>
// # include "LocationRules.hpp"
# include <fstream>
# include <string>
# include <cstdlib>

class ServerConfig
{
private:
    int                                     _host;
    int                                     _port;
    // std::vector<std::string>                _serverName;
    // std::string                             _root;
    // std::map<int, std::string>      _errorPages;
    // int                                     _maxBodySize;
    // std::map<std::string, LocationRules>    _locations; //Mapa donde almacenamos la info de las locations
    std::list<std::string>                  _serverNames;
    // std::map<std::string, LocationRules *>    _locations; //Mapa donde almacenamos la info de las locations

public:
    ServerConfig();
    ~ServerConfig();
    void setHost(unsigned int host);
    void setPort(unsigned int port);
    void setServerName(std::string serverName);
    int getPort()const;
    std::list<std::string> getServerNames();
};

#endif
