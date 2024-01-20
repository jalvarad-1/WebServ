#include "LocationRules.hpp"

LocationRules::LocationRules() {
    std::cout << "LocationRules constructor called" << std::endl;
}

LocationRules::LocationRules(std::string key_value,std::map<int,
    std::string> error_pages, std::string root, int maxBodySize,
    std::list<std::string> allowedMethods, bool dir_list, std::string default_file, std::string cgi_pass)
{
    this->_key_value = key_value;
    this->_error_pages = error_pages;
    this->_root = root;
    this->_maxBodySize = maxBodySize;
    this->_allowedMethods = allowedMethods;
    this->_dir_list = dir_list;
    this->_default_file = default_file;
    this->_cgi_pass = cgi_pass;
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

std::string LocationRules::getKeyValue() const
{
    return this->_key_value;
}

std::string LocationRules::getCgiPass() const
{
    return this->_cgi_pass;
}

LocationRules::~LocationRules() {}