#include "LocationRules.hpp"

LocationRules::LocationRules() {
    std::cout << "LocationRules constructor called" << std::endl;
}

std::map<int, std::string>  LocationRules::getErrorPages() const
{
    return this->_error_pages;
}

std::string LocationRules::getRoot() const
{
    return this->_root;
}

int LocationRules::getMaxBodySize() const
{
    return this->_maxBodySize;
}

std::list<std::string> LocationRules::getAllowedMethods() const
{
    return this->_allowedMethods;
}

bool LocationRules::getDirList() const
{
    return this->_dir_list;
}

std::string LocationRules::getDefaultFile() const
{
    return this->_default_file;
}

LocationRules::~LocationRules() {}