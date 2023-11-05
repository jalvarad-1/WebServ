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

ConfigParser::~ConfigParser() {}
