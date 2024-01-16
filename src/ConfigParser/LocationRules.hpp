#ifndef LocationRules_hpp
# define LocationRules_hpp

# include <iostream>
# include <list>
# include <map>

class LocationRules
{
private:
    std::map<int, std::string>  _error_pages;
    std::string                         _root;
    int                                 _maxBodySize;
    std::list<std::string>              _allowedMethods;
    bool                                _dir_list;
    std::string                         _default_file;
    std::string                         _cgi_pass;
    //redirect loquesea.com  -> debe enviar status 300 y además un header con Location y el redirect
    //listing directory on or of
    //Set a default file to answer if the request is a directory.

public:
    LocationRules();
    ~LocationRules();

    std::map<int, std::string>  getErrorPages() const;
    std::string getRoot() const;
    int getMaxBodySize() const;
    std::list<std::string> getAllowedMethods() const;
    bool getDirList() const;
    std::string getDefaultFile() const;
};

#endif