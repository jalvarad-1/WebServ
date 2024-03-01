#ifndef LocationRules_hpp
# define LocationRules_hpp

# include <iostream>
# include <list>
# include <map>
# include <algorithm>

class LocationRules
{
private:
    std::string                         _key_value;
    std::map<int, std::string>          _error_pages;
    std::string                         _root;
    int                                 _maxBodySize;
    std::list<std::string>              _allowedMethods;
    bool                                _auto_index;
    std::string                         _index;
    std::string                         _cgi_pass;
    std::string                         _cgi_extension;
    std::string                         _delete_pass;
    std::string                         _redirect;

public:
    LocationRules();
    ~LocationRules();
    
    std::map<int, std::string>  getErrorPages() const;
    std::string getRoot() const;
    int getMaxBodySize() const;
    std::list<std::string> & getAllowedMethods();
    bool isAuto_index() const;
    std::string getIndex() const;
    std::string getCgiExtension() const;
    std::string getCgiPass() const;
    std::string getDelPass() const;
    std::string getKeyValue() const;
    std::string getRedirect() const;

    void setErrorPage(int error_code, std::string error_page);
    bool setRoot(std::string root);
    bool setIndex(std::string index);
    bool setMaxBodySize(int maxBodySize);
    void setAutoIndex(bool auto_index);
    bool setAllowedMethod(std::string allowedMethod);
    bool setCGIpass(std::string cgi_extension, std::string cgi_pass);
    bool setDelpass(std::string delete_pass);
    void setKeyValue(std::string key_value);
    bool setRedirect(std::string redirect);

    void printAttributes() const;
};

#endif
