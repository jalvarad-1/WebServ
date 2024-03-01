#ifndef RESPONSECODE_HPP
# define RESPONSECODE_HPP

# include <map>
# include <iostream>

class ResponseCode
{
private:
    std::map<int, std::string> _codes;
public:
    std::string getCodeString(int code);
    ResponseCode();
    ~ResponseCode();
};

#endif
