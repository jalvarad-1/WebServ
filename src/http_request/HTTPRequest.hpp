#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

# include <string>
# include <map>
# include <cstdlib>

#define METHODS_WITH_BODY "POST"

inline int ñapaCounter;

class HTTPRequest {
    private:
        std::string _method;
        std::string _uri;
        std::string _http_version;
        std::map<std::string, std::string> _headers;
        std::string _error_message;

    public:
        std::string _body;
        HTTPRequest(void);
        HTTPRequest(const std::string& raw_request);

        bool parse(const std::string& raw_request); // true = valid request, false = invalid request
        bool methodAcceptsBody() const;
        int returnContentLength() const;
        std::string getMethod() const;
        std::string getURI() const;
        std::string getVersion() const;
        std::string getHeader(const std::string& header_name) const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        std::string getErrorMessage() const;
};

#endif
