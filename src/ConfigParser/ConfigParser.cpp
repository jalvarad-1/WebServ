#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const char * path) {
    std::ifstream myFile;
    std::streampos confInit, confActual;
    myFile.open(path);

    if (!myFile.is_open()) {
        throw std::runtime_error("Config file not found");
    }
    else {
        int position;
        int open = 0;
        size_t pos;
        std::string line;
        while (std::getline(myFile, line)) {
            pos = line.find("server {");
            if (pos != std::string::npos) {
                confInit = myFile.tellg();
                position = 0;
                open++;
            }
            else {
                pos = line.find("{");
                if (pos != std::string::npos) {
                    open++;
                }
                else
                {
                    pos = line.find("}");
                    if (pos != std::string::npos) {
                        if (open == 1) {
                            confActual = myFile.tellg();
                            myFile.seekg(confInit);
                            this->confServers.push_back(ServerConfig(myFile, position));
                            myFile.seekg(confActual);
                        }
                        open--;
                    }
                }
            }
            position++;
        }
        if (open != 0) {
            throw std::runtime_error("Config file not valid");
        }
        myFile.close();
    }
}

ConfigParser::~ConfigParser() {}
