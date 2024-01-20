#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(int host, int port, std::list<std::string> serverNames,
                    std::map<std::string, LocationRules *> locations)
{
    this->_host = host;
    this->_port = port;
    this->_serverNames = serverNames;
    this->_locations = locations;
}

void    ServerConfig::setHost(unsigned int host) {
    this->_host = host;
}

void    ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}

void    ServerConfig::setServerName(std::string serverName) {
    this->_serverNames.push_back(serverName);
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

std::map<std::string, LocationRules *>    ServerConfig::getLocations() const
{
    return this->_locations;
}

ServerConfig::~ServerConfig() {}

