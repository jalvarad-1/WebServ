#ifndef utils_hpp
# define utils_hpp

# include <sstream>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>

    bool isDigitString(const std::string& str);
    bool deleteChar(std::string &texto, char caracterABuscar);
    std::vector<std::string> split(std::string input);
    std::vector<std::string> split_char(std::string input, char delimiter);
    std::string readFromFile(std::string filename);
    
#endif
