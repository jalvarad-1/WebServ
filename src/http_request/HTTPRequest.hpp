#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

# include <string>
# include <map>
# include <cstdlib>
# include "../server/routing/Routing_ns.hpp"

#define METHODS_WITH_BODY "POST"

class HTTPRequest {
    private:
        std::string _method;
        std::string _uri;
        std::string _http_version;
        std::map<std::string, std::string> _headers;
        int _error_code;
        std::string _path_info;
        LocationRules * _location_rules;
        std::string _file_path;
        
        

    public:
		std::string _body_file_name;
		int			_body_file_fd;
        std::string _body;
        HTTPRequest(void);
        HTTPRequest(const std::string& raw_request, ServerConfig& serverConfig);

        bool parse(const std::string& raw_request); // true = valid request, false = invalid request
        bool methodAcceptsBody() const;
        int returnContentLength() const;
        std::string getMethod() const;
        std::string getURI() const;
        std::string getVersion() const;
        std::string getHeader(const std::string& header_name) const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        int getErrorCode() const;
        std::string getPathInfo() const;
        void setPathInfo(std::string path_info);
        std::string getFilePath() const;
};

#endif
