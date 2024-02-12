#ifndef Response_hpp
#define Response_hpp
# include <map>
# include <string>

struct Response {
    std::map<std::string, std::string> headers;
    std::string     string_body;
    std::string     file_path;
    int             response_code;
};

#endif
