#include "ConfigParser.hpp"
#include <sstream>

bool isDigitString(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
    return true;
}

bool deleteChar(std::string &texto, char caracterABuscar) {
    size_t pos = texto.find(caracterABuscar);

    if (pos != std::string::npos) {
        texto.erase(pos, 1);
        return true;
    }

    return false;
}

std::vector<std::string> split2(std::string input) {
    std::vector<std::string> palabras;
    std::istringstream iss(input);
    std::string palabra;

    while (iss >> palabra) {
        palabras.push_back(palabra);
    }

    return palabras;
}

int goodServDeclaration(std::vector<std::string> splitLine) {
    if (splitLine.size() > 0 && splitLine[0] == "server") {
        if (splitLine[1] == "{") {
            return (1);
        }
        else
            return (-1);
    }
    else if (splitLine.size() > 0 && splitLine[0] == "server{") {
        return(1);
    }
    else {
        return (0);
    }
}

std::vector<std::streampos> findServers(std::ifstream& myFile) {
    std::vector<std::streampos> serverInit;
    std::vector<std::string>    splitLine;
    int                         isServer;
    std::string                 line;

    while (std::getline(myFile, line)) {
        splitLine = split2(line);
        if (line != "") {
            isServer = goodServDeclaration(splitLine);
            if (isServer == 1)
                serverInit.push_back(myFile.tellg());
            else if (isServer == -1)
                throw std::runtime_error("Invalid server definition.");
        }
    }
    return serverInit;
}

void    setServerPort(std::vector<std::string> splitLine, ServerConfig& servidor) {
    if (splitLine.size() == 1)
        throw std::runtime_error("Port definition must have a port.");
    else if (splitLine[1].find(";") == std::string::npos)
        throw std::runtime_error("Port definition must finish with ';'.");
    else {
        deleteChar(splitLine[1], ';');
        if (isDigitString(splitLine[1]) == false) {
            throw std::runtime_error("Port definition must be a valid number.");
        }
        else
            servidor.setPort(std::atoi(splitLine[1].c_str()));
    }
}

void setServerName(std::vector<std::string> splitLine, ServerConfig& servidor) {
    if (splitLine.size() == 1)
        throw std::runtime_error("Empty server_name definition.");
    else if (splitLine[1].find(";") == std::string::npos)
        throw std::runtime_error("Server_name definition must finish with ';'.");
    else {
        deleteChar(splitLine[1], ';');
        servidor.setServerName(splitLine[1]);
    }
}

void    setValues(std::vector<std::string> splitLine, ServerConfig& servidor) {
    if (splitLine[0] == "listen")
        setServerPort(splitLine, servidor);
    else if (splitLine[0] == "server_name")
        setServerName(splitLine, servidor);
}

void    saveServer(std::ifstream& myFile, std::vector<ServerConfig>& confServers) {
    std::vector<std::string>    splitLine;
    int                         isServer;
    int                         open = 1;
    std::string                 line;
    ServerConfig                servidor;

    myFile.clear();
    while (std::getline(myFile, line)) {
        if (line != "") {
            splitLine = split2(line);
            isServer = goodServDeclaration(splitLine);
            if (isServer != 0)
                throw std::runtime_error("Invalid server redefinition.");
            if (line.find("}") != std::string::npos) {
                open--;
                if (open == 0)
                    break;
            } else if (line.find("{") != std::string::npos)
                open++;
            setValues(splitLine, servidor);
        }
    }
    if (open > 0)
        throw std::runtime_error("Server definition not closed.");
    confServers.push_back(servidor);
    (void)open;
    (void)confServers;
}

ConfigParser::ConfigParser(const char * path) {
    std::vector<std::streampos> serverPosition;
    std::vector<std::string>    fileSaved;
    std::ifstream               myFile;

    std::string                 line;
    
    myFile.open(path);
    if (!myFile.is_open()) {
        throw std::runtime_error("Config file not found");
    }
    else {
        serverPosition = findServers(myFile);
        myFile.clear();

        for (size_t i = 0; i < serverPosition.size(); i++) {
            myFile.seekg(serverPosition[i]);
            saveServer(myFile, this->confServers);
        }
        myFile.close();
        for (size_t i = 0; i < confServers.size(); i++) {
            std::cout << "Port: " << confServers[i].getPort() << std::endl;
        }
    }
}

std::vector<ServerConfig> ConfigParser::getConfigServers() {
    return this->confServers;
}

ConfigParser::~ConfigParser() {}
