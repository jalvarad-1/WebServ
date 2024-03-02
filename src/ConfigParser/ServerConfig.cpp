#include "ServerConfig.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ServerConfig::ServerConfig() {
    _empty_server = true;
}

void    ServerConfig::setServerName(std::string serverName) {
    if (std::find(_serverNames.begin(), _serverNames.end(), serverName) == this->_serverNames.end())
        _serverNames.push_back(serverName);
}

int     ServerConfig::getHost() const
{
    return this->_host;
}

int     ServerConfig::getPort() const{
    return this->_port;
}

std::list<std::string>    ServerConfig::getServerNames() {
    return this->_serverNames;
}

ServerConfig::~ServerConfig() {

}

bool ServerConfig::setHostAndPort(std::string & hostAndPort){
    if (!_empty_server)
        return false;
    
    std::string host, port;
    std::size_t pos = hostAndPort.find(':');
    if (pos == std::string::npos)
        return false;
    host = hostAndPort.substr(0, pos);
    port = hostAndPort.substr(pos + 1);
    this->_host = inet_addr(host.c_str());
    pos = 0;
    if (port.find_first_not_of("0123456789") != std::string::npos || port.empty())
        return false;
    this->_port = std::atoi(port.c_str());
    // INADDR_NONE is -1 and 255.255.255.255 is -1, but is a valid ip
    if ((this->_host == INADDR_NONE && host != "255.255.255.255") || this->_port < 0)
        return false;

    _empty_server = false;
    return true;
}


void ServerConfig::printServerConfig() const {
        std::cout << "Host: " << _host << std::endl;
        std::cout << "Port: " << _port << std::endl;
        std::cout << "Server Names: ";
        for (std::list<std::string>::const_iterator it = _serverNames.begin(); it != _serverNames.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        std::cout << "\n-----LOCATIONS-----" << std::endl;
        for (std::map<std::string, LocationRules>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
            std::cout << "--------------------------------" << std::endl;
            std::cout << "--Location Key: " << it->first << std::endl;
            it->second.printAttributes();
        }
    }

bool ServerConfig::isEmptyServer() const {
    return _empty_server;
}
