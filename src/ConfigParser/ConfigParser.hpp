#ifndef ConfitParser_hpp
# define ConfigParser_hpp

# include "ServerConfig.hpp"
# include <iostream>
# include <vector>
# include <fstream>
# include <string>

class ConfigParser
{
    private:
        std::vector<ServerConfig> confServers;
    public:
        ConfigParser(const char * path);
        ~ConfigParser();
};

#endif