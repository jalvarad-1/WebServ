#include "HTTPRequest.hpp"
#include <sstream>
#include <iostream>

void trim(std::string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");

    if (start == std::string::npos)
        s = "";
    else
        s = s.substr(start, end - start + 1);
}

HTTPRequest::HTTPRequest(const std::string& raw_request)
{
    if (!parse(raw_request))
        _error_message = "Failed to parse the request.";
}

bool HTTPRequest::methodAcceptsBody(const std::string& method) const
{
        std::string methods = METHODS_WITH_BODY;
        return methods.find(method) != std::string::npos;
}

bool HTTPRequest::parse(const std::string& raw_request) {
    std::istringstream ss(raw_request);
    std::string line;

    // 2. Analiza la línea de solicitud (por ejemplo: GET /index.html HTTP/1.1)
    if (!std::getline(ss, line))
        return false;
    std::istringstream request_line(line);
    if (!(request_line >> _method >> _uri >> _http_version))
        return false;

    while (std::getline(ss, line) && !line.empty() && line != "\r")
    {
        std::size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
        {
            std::cout << "1" << std::endl;
            return false;
        }

        std::string header_name = line.substr(0, colon_pos);
        std::string header_value = line.substr(colon_pos + 1);

        trim(header_name);
        trim(header_value);

        if (!header_name.empty() && !header_value.empty())
            _headers[header_name] = header_value;
        else
        {
            std::cout << "1" << std::endl;
            return false;
        }
    }
    
    if (methodAcceptsBody(_method))
    {
        // se utiliza ostringstream por temas de performance, ya que no creas una nueva cadena en cada iteración como se haría si hiciera _body += line
        std::ostringstream body_stream;
        while (std::getline(ss, line))
            body_stream << line << "\n";
        _body = body_stream.str();

    }
    return true;// devuelve true si todo salió bien, o false si hubo algún error
}

std::string HTTPRequest::getMethod() const
{
    return _method;
}

std::string HTTPRequest::getURI() const {
    return _uri;
}

std::string HTTPRequest::getVersion() const {
    return _http_version;
}

std::string HTTPRequest::getHeader(const std::string& header_name) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(header_name);
    if (it != _headers.end()) {
        return it->second;
    }
    return "";
}

std::string HTTPRequest::getBody() const
{
    return _body;
}

std::string HTTPRequest::getErrorMessage() const
{
        return _error_message;
}

std::map<std::string, std::string> HTTPRequest::getHeaders() const {
    return _headers;
}
