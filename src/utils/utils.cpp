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
    std::vector<std::string> words;
    std::istringstream iss(input);
    std::string word;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}

std::vector<std::string> splitAndIncludeSpecialChars(std::string input) {
    std::vector<std::string> words;
    std::istringstream iss(input);
    std::string word;

    while (iss >> word) {
        std::string segment;
        for (std::string::size_type i = 0; i < word.size(); ++i) {
            char ch = word[i];
            if (ch == '{' || ch == '}' || ch == ';') {
                if (!segment.empty()) {
                    words.push_back(segment);
                    segment.clear();
                }
                words.push_back(std::string(1, ch));
            } else {
                segment += ch;
            }
        }
        if (!segment.empty()) {
            words.push_back(segment);
        }
    }
    return words;
}
