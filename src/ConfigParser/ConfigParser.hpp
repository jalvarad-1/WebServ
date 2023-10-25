#ifndef ConfitParser_hpp
# define ConfigParser_hpp

# include "ServerConfig.hpp"
# include <iostream>
# include <vector>

class ConfigParser
{
    private:
        std::vector<ServerConfig> confServers;
    public:
        ConfigParser(std::string path);
        ~ConfigParser();
};


#endif