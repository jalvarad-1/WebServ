#include "HTTPResponse.hpp"
#include "HTTPResponse.hpp"
#include <sstream>

HTTPResponse::HTTPResponse(const ServerConfig &server_config, const HTTPRequest &request) {
    constructResponse(server_config, request);
}

void HTTPResponse::constructResponse(const ServerConfig &server_config, const HTTPRequest &request) {
    // Aquí validas la solicitud y construyes la respuesta
    // Por ejemplo, puedes verificar el método HTTP, la URI, los encabezados, etc.
    // Y establecer _status_code, _status_message, _headers, _body, etc. según sea necesario

    // Por ejemplo:
    _http_version = request.getVersion();
    _status_code = 200;  // OK
    _status_message = "OK";
    _headers["Content-Type"] = "text/html";
    _body = "<html><body>Hello, world!</body></html>";

    // Construir la respuesta HTTP en formato raw
    std::ostringstream response_stream;
    response_stream << _http_version << " " << _status_code << " " << _status_message << "\r\n";
    for (const auto &header : _headers) {
        response_stream << header.first << ": " << header.second << "\r\n";
    }
    response_stream << "\r\n" << _body;
    _response_raw = response_stream.str();
}

// Implementa aquí los métodos para obtener partes de la respuesta HTTP...

std::string HTTPResponse::getRawResponse() const {
    return _response_raw;
}
