#ifndef RequestLineException_hpp
#define RequestLineException_hpp
#include <string>
#include <stdexcept>

class RequestLineException: public std::exception
{
    private:
        const char * _fail;
    public:
        RequestLineException(const char * exception);
        virtual const char* what() const throw();
};

#endif
