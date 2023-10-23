#include "RequestLineException.hpp"

RequestLineException::RequestLineException(const char * exception): _fail(exception)
{
}

const char* RequestLineException::what() const throw()
{
    return _fail;
}
