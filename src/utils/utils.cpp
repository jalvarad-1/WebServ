#include "utils.hpp"

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

std::vector<std::string> split(std::string input) {
    std::vector<std::string> palabras;
    std::istringstream iss(input);
    std::string palabra;

    while (iss >> palabra) {
        palabras.push_back(palabra);
    }

    return palabras;
}

std::vector<std::string> split_char(std::string input, char delimiter) {
    std::vector<std::string> palabras;
    std::istringstream iss(input);
    std::string palabra;

    while (std::getline(iss, palabra, delimiter)) {
        palabras.push_back(palabra);
    }

    return palabras;
}