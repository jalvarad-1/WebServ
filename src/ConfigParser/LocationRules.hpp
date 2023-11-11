#ifndef LocationRules_hpp
# define LocationRules_hpp

# include <iostream>
# include <list>

class LocationRules
{
private:
    int maxBodySize;
    std::list<std::string> allowedMethods;
    //redirect loquesea.com  -> debe enviar stauts 300 y además un header con Location y el redirect
    //listing directory on or of
    //Set a default file to answer if the request is a directory.

public:
    LocationRules();
    ~LocationRules();
};

#endif