#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

void    ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}

void    ServerConfig::setServerName(std::string serverName) {
    this->_serverName.push_back(serverName);
}

void    ServerConfig::setRoot(std::string root) {
    this->_root = root;
}

void    ServerConfig::setErrorPages(int err_code, std::string page_body) {
    this->_errorPages[err_code] = page_body;
}

std::string ServerConfig::getErrorBody(int err_code) {
    return this->_errorPages[err_code];
}

int     ServerConfig::getPort() const{
    return this->_port;
}

std::vector<std::string>    ServerConfig::getServerName() {
    return this->_serverName;
}

std::string ServerConfig::getRoot() {
    return this->_root;
}

ServerConfig::~ServerConfig() {}

