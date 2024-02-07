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
    std::list<std::string>              _allowedMethods; //AUTO_INDEX
    bool                                _auto_index;
    std::string                         _index;
    std::string                         _cgi_pass;
    std::string                         _cgi_extension;
    //redirect loquesea.com  -> debe enviar status 300 y además un header con Location y el redirect

public:
    LocationRules();
    ~LocationRules();
    //temporary constructor
    LocationRules(std::string key_value, std::map<int,
                std::string>  error_pages, std::string root, int maxBodySize,
                std::list<std::string> allowedMethods, bool auto_index,
                std::string index, std::string cgi_pass);
    std::map<int, std::string>  getErrorPages() const;
    std::string getRoot() const;
    int getMaxBodySize() const;
    std::list<std::string> & getAllowedMethods();
    bool isAuto_index() const;
    std::string getIndex() const;
    std::string getCgiExtension() const;
    std::string getCgiPass() const;
    std::string getKeyValue() const;

    void setErrorPage(int error_code, std::string error_page);
    bool setRoot(std::string root);
    bool setIndex(std::string index);
    bool setMaxBodySize(int maxBodySize);
    void setAutoIndex(bool auto_index);
    bool setAllowedMethod(std::string allowedMethod);
    bool setCGIpass(std::string cgi_extension, std::string cgi_pass);
    void setKeyValue(std::string key_value);

    void printAttributes() const;
};

#endif
