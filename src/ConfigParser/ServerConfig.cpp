#include "ServerConfig.hpp"
#include <sstream>

bool isDigitString(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> split(std::string input) {
    std::vector<std::string> palabras;
    std::istringstream iss(input);
    std::string palabra;

    while (iss >> palabra) {
        palabras.push_back(palabra);
    }

    return palabras;
}

bool eliminarCaracter(std::string &texto, char caracterABuscar) {
    size_t pos = texto.find(caracterABuscar);

    if (pos != std::string::npos) {
        texto.erase(pos, 1);
        return true;
    }

    return false;
}

void findPort(std::string line, int& port) {
    size_t pos;
    pos = line.find("listen");
    if (pos != std::string::npos) {
        std::vector<std::string> port_split = split(line);
        if (eliminarCaracter(port_split[1], ';')) {
            if (isDigitString(port_split[1]))
                port = std::atoi(port_split[1].c_str());
            else
                throw std::runtime_error("Port must be a number");
        }
        else {
            throw std::runtime_error("Expected ';' in port declaration");
        }
    }
}

void findServerName(std::string line, std::vector<std::string>& serverName) {
    size_t pos;
    size_t i = 1;
    pos = line.find("server_name");
    if (pos != std::string::npos) {
        std::vector<std::string> name_split = split(line);
        while (i < name_split.size()) {
            if (i == name_split.size() - 1) {
                if (!eliminarCaracter(name_split[i], ';'))
                    throw std::runtime_error("Expected ';' in server_name declaration");
            }
            if (eliminarCaracter(name_split[i], ';')) {
                serverName.push_back(name_split[i]);
                break;
            }
            else {
                serverName.push_back(name_split[i]);
            }
            i++;
        }
    }
    (void)line;
    (void)serverName;
}

ServerConfig::ServerConfig(std::ifstream& myFile, int position) {
    std::string line;
    int lineCounter = 0;
    
    while (lineCounter < position) {
        std::getline(myFile, line);
        findPort(line, this->port);
        findServerName(line, this->serverName);
        lineCounter++;
    }
}

int ServerConfig::getPort() {
    return this->port;
}

std::vector<std::string> ServerConfig::getServerName() {
    return this->serverName;
}

ServerConfig::~ServerConfig() {}

