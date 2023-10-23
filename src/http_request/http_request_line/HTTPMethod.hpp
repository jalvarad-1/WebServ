#ifndef HTTPMethod_hpp
#define HTTPMethod_hpp
#include <string>

class Method
{
    private:
        std::string _method;
    
    public:
        bool parse();
        
};

#endif
