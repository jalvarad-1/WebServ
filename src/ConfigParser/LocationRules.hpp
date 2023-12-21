#ifndef LocationRules_hpp
# define LocationRules_hpp

# include <iostream>
# include <list>

class LocationRules
{
private:
    std::map<std::string, std::string>  _error_pages;
    std::string                         _root;
    int                                 _maxBodySize;
    std::list<std::string>              _allowedMethods;
    bool                                _dir_list;
    std::string                         _default_file;
    //redirect loquesea.com  -> debe enviar stauts 300 y además un header con Location y el redirect
    //listing directory on or of
    //Set a default file to answer if the request is a directory.

public:
    LocationRules();
    ~LocationRules();
};

#endif