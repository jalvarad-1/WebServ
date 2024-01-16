#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

void    ServerConfig::setHost(unsigned int host) {
    this->_host = host;
}

void    ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}

void    ServerConfig::setServerName(std::string serverName) {
    this->_serverNames.push_back(serverName);
}


int     ServerConfig::getPort() const{
    return this->_port;
}

std::list<std::string>    ServerConfig::getServerNames() {
    return this->_serverNames;
}

ServerConfig::~ServerConfig() {}

