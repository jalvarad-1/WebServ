#include "HTTPRequest.hpp"
#include <sstream>

HTTPRequest::HTTPRequest(const std::string& raw_request) {
    parse(raw_request);
}

bool HTTPRequest::parse(const std::string& raw_request) {
    // 1. Divide la solicitud en líneas
    std::istringstream ss(raw_request);
    std::string line;

    // 2. Analiza la línea de solicitud (por ejemplo: GET /index.html HTTP/1.1)
    std::getline(ss, line);
    std::istringstream request_line(line);
    request_line >> method >> uri >> http_version;

    if (method.empty() || uri.empty() || http_version.empty()) {
        return false;
    }

    // 3. Analiza las cabeceras
    while (std::getline(ss, line) && line != "\r") { 
        std::size_t sep = line.find(":");
        if (sep != std::string::npos) {
            std::string header_name = line.substr(0, sep);
            std::string header_value = line.substr(sep + 1);
            headers[header_name] = header_value;
        }
    }

    // 4. Realiza cualquier otra validación que desees aquí.

    return true; // devuelve true si todo salió bien, o false si hubo algún error
}

std::string HTTPRequest::getMethod() const {
    return method;
}

std::string HTTPRequest::getURI() const {
    return uri;
}

std::string HTTPRequest::getVersion() const {
    return http_version;
}

std::string HTTPRequest::getHeader(const std::string& header_name) const {
    std::map<std::string, std::string>::const_iterator it = headers.find(header_name);
    if (it != headers.end()) {
        return it->second;
    }
    return "";
}
