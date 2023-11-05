#include "ConfigParser.hpp"

int findBlock(std::ifstream& myFile, std::string blockStart, std::vector<ServerConfig>& confServers) {
    int open = 0;
    int position;
    std::streampos confInit, confActual;
    std::string line;
    while (std::getline(myFile, line)) {
        if (line.find(blockStart) != std::string::npos) {
            confInit = myFile.tellg();
            position = 0;
            open++;
        }
        else if (line.find("{") != std::string::npos)
            open++;
        else if (line.find("}") != std::string::npos) {
            if (open == 1) {
                confActual = myFile.tellg();
                myFile.seekg(confInit);
                confServers.push_back(ServerConfig(myFile, position));
                std::cout << "Server added" << std::endl;
                myFile.seekg(confActual);
            }
            open--;
        }
        position++;
    }
    std::cout << confServers[0].getPort() << std::endl;
    std::cout << confServers[1].getPort() << std::endl;
    std::cout << confServers[2].getPort() << std::endl;
    return open;
}

ConfigParser::ConfigParser(const char * path) {
    std::ifstream myFile;
    myFile.open(path);

    if (!myFile.is_open()) {
        throw std::runtime_error("Config file not found");
    }
    else {
        if (findBlock(myFile, "server {", this->confServers) != 0) {
            throw std::runtime_error("Config file not valid");
        }
        myFile.close();
    }
}

std::vector<ServerConfig> ConfigParser::getConfigServers() {
    return this->confServers;
}

ConfigParser::~ConfigParser() {}
