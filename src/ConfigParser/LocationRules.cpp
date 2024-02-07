#include "LocationRules.hpp"

LocationRules::LocationRules() {
    _key_value = "default";
    _root = "";
    _maxBodySize = 0; //// if 0 is setted, there is no limit
    _auto_index = false;
    _index = "";
    _cgi_pass = "";
    _cgi_extension = "";
}

LocationRules::LocationRules(std::string key_value,std::map<int,
    std::string> error_pages, std::string root, int maxBodySize,
    std::list<std::string> allowedMethods, bool auto_index, std::string index, std::string cgi_pass)
{
    this->_key_value = key_value;
    this->_error_pages = error_pages;
    this->_root = root;
    this->_maxBodySize = maxBodySize;
    this->_allowedMethods = allowedMethods;
    this->_auto_index = auto_index;
    this->_index = index;
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

std::list<std::string> & LocationRules::getAllowedMethods()
{
    return this->_allowedMethods;
}

bool LocationRules::isAuto_index() const
{
    return this->_auto_index;
}

std::string LocationRules::getIndex() const
{
    return this->_index;
}

std::string LocationRules::getKeyValue() const
{
    return this->_key_value;
}

std::string LocationRules::getCgiPass() const
{
    return this->_cgi_pass;
}

std::string LocationRules::getCgiExtension() const
{
    return this->_cgi_extension;
}

LocationRules::~LocationRules() {}

void LocationRules::setErrorPage(int error_code, std::string error_page)
{
    this->_error_pages[error_code] = error_page;
}

bool LocationRules::setRoot(std::string root)
{
    if (_root != "")
        return false;
    this->_root = root;
    return true;
}

bool LocationRules::setIndex(std::string index)
{
    if (_index != "")
        return false;
    this->_index = index;
    return true;
}

bool LocationRules::setMaxBodySize(int maxBodySize)
{
    if (maxBodySize < 0 || _maxBodySize != 0)
        return false;
    this->_maxBodySize = maxBodySize;
    return true;
}

void LocationRules::setAutoIndex(bool auto_index)
{
    this->_auto_index = auto_index;
}

bool LocationRules::setAllowedMethod(std::string allowedMethod)
{
    if (allowedMethod == "GET" || allowedMethod == "POST" || allowedMethod == "DELETE") {
        if (std::find(_allowedMethods.begin(), _allowedMethods.end(), allowedMethod) == _allowedMethods.end())
            _allowedMethods.push_back(allowedMethod);
        return true;
    }
    return false;
}

bool LocationRules::setCGIpass(std::string cgi_extension, std::string cgi_pass)
{
    if (_cgi_pass != "" || _cgi_extension != "")
        return false;
    if (cgi_extension[0] != '.' || cgi_extension.length() < 2 || cgi_pass.empty())
        return false;
    _cgi_pass = cgi_pass;
    _cgi_extension = cgi_extension;
    return true;
}

void LocationRules::setKeyValue(std::string key_value)
{
    _key_value = key_value;
}

void LocationRules::printAttributes() const {
        std::cout << "  Key Value: " << _key_value << std::endl;
        std::cout << "  Root: " << _root << std::endl;
        std::cout << "  Max Body Size: " << _maxBodySize << std::endl;
        std::cout << "  Auto Index: " << (_auto_index ? "true" : "false") << std::endl;
        std::cout << "  Index: " << _index << std::endl;
        std::cout << "  CGI Pass: " << _cgi_pass << std::endl;
        std::cout << "  CGI Extension: " << _cgi_extension << std::endl;

        std::cout << "  Error Pages:" << std::endl;
        for (std::map<int, std::string>::const_iterator it = _error_pages.begin(); it != _error_pages.end(); ++it) {
            std::cout << "  Code: " << it->first << ", Page: " << it->second << std::endl;
        }

        std::cout << "  Allowed Methods:" << std::endl;
        for (std::list<std::string>::const_iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); ++it) {
            std::cout << "  Method: " << *it << std::endl;
        }
}