#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

#include <string>
#include <map>

class HTTPRequest {
    private:
        std::string method;
        std::string uri;
        std::string http_version;
        std::map<std::string, std::string> headers;
        int status;

    public:
        HTTPRequest(const std::string& raw_request);

        bool parse(const std::string& raw_request); // true = valid request, false = invalid request    
        std::string getMethod() const;
        std::string getURI() const;
        std::string getVersion() const;
        std::string getHeader(const std::string& header_name) const;
};

// Esta http request molaría que reciba un ConfHTTPServer
#endif
