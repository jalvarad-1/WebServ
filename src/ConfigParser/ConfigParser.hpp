#ifndef ConfitParser_hpp
# define ConfigParser_hpp

# include "ServerConfig.hpp"
# include <iostream>
# include <vector>
# include <fstream>
# include <string>
# include <stdexcept>

# include "../utils/utils.hpp"


class ConfigParser
{
    private:
        std::vector<ServerConfig> confServers;
    public:
        std::vector<ServerConfig> getConfigServers();
        ConfigParser(const char * path);
        ~ConfigParser();
};

#endif