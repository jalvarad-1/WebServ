#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

void ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}

void ServerConfig::setServerName(std::string serverName) {
    this->_serverName.push_back(serverName);
}

int ServerConfig::getPort() {
    return this->_port;
}

std::vector<std::string> ServerConfig::getServerName() {
    return this->_serverName;
}

ServerConfig::~ServerConfig() {}

