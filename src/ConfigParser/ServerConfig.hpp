#ifndef ServerConfig_hpp
# define ServerConfig_hpp

# include <iostream>
# include <map>
# include <vector>
// # include "LocationRules.hpp"
# include <fstream>
# include <string>
# include <cstdlib>

class ServerConfig
{
private:
    int                                     _port;
    std::vector<std::string>                _serverName;
    std::string                             _root;
    std::map<int, std::string>      _errorPages;
    int                                     _maxBodySize;
    // std::map<std::string, LocationRules>    _locations; //Mapa donde almacenamos la info de las locations

public:
    ServerConfig();
    ~ServerConfig();
    void setPort(unsigned int port);
    void setServerName(std::string serverName);
    void setRoot(std::string root);
    void setErrorPages(int err_code, std::string page_body);
    std::string getErrorBody(int err_code);
    int getPort()const;
    std::vector<std::string> getServerName();
    std::string getRoot();
};

#endif
