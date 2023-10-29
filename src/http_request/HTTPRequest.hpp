#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

#include <string>
#include <map>

#define METHODS_WITH_BODY "POST"

class HTTPRequest {
    private:
        std::string _method;
        std::string _uri;
        std::string _http_version;
        std::map<std::string, std::string> _headers;
        std::string _error_message;
        std::string _body;

    public:
        HTTPRequest(const std::string& raw_request);

        bool parse(const std::string& raw_request); // true = valid request, false = invalid request
        bool methodAcceptsBody(const std::string& method) const;
        std::string getMethod() const;
        std::string getURI() const;
        std::string getVersion() const;
        std::string getHeader(const std::string& header_name) const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        std::string getErrorMessage() const;
};

#endif
