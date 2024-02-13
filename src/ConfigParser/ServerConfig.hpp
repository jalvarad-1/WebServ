#ifndef ServerConfig_hpp
# define ServerConfig_hpp

# include <iostream>
# include <map>
# include <vector>
# include <list>
# include "LocationRules.hpp"
# include <fstream>
# include <string>
# include <cstdlib>

class ServerConfig
{
private:
    unsigned int                            _host;
    int                                     _port;
    bool                                    _empty_server;                         
    std::list<std::string>                  _serverNames;

public:
    ServerConfig();
    ~ServerConfig();

    std::map<std::string, LocationRules>  locations;
    
    void setHost(unsigned int host);
    void setPort(unsigned int port);
    void setServerName(std::string serverName);
    int getHost()const;//No tengo claro que host nos sirva para algo, es necesario?
    int getPort()const;
    std::list<std::string> getServerNames();

    bool setHostAndPort(std::string & hostAndPort);
    void printServerConfig() const;
    bool isEmptyServer() const;
};

#endif
