#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const char * path) {
    std::cout << path << std::endl;
    std::ifstream myFile;
    myFile.open(path);

    if (!myFile.is_open()) {
        std::cout << "Error. Config file not found" << std::endl;
        //Aquí el código debería de lanzar una excepcion.
    }
    else {
        std::string line;
        while (std::getline(myFile, line)) {
            std::cout << line << std::endl;
        }
        myFile.close();
    }
}

ConfigParser::~ConfigParser() {}
