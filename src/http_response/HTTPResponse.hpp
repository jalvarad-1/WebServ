#ifndef HTTPResponse_hpp
#define HTTPResponse_hpp

#include "MockConfigFile.hpp"
#include "HTTPRequest.hpp"
#include <sstream>
#include <string>
#include <map>

class HTTPResponse {
private:
    std::string _http_version;
    int _status_code;
    std::string _status_message;
    std::map<std::string, std::string> _headers;
    std::string _body;
    std::string _response_raw;

    // Método para validar y construir la respuesta
    void constructResponse(const ServerConfig &server_config, const HTTPRequest &request);

public:
    // Constructor que toma la configuración del servidor y la solicitud HTTP
    HTTPResponse(const ServerConfig &server_config, const HTTPRequest &request);

    // Métodos para obtener partes de la respuesta HTTP
    std::string getVersion() const;
    int getStatusCode() const;
    std::string getStatusMessage() const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getBody() const;

    // Método para obtener la respuesta HTTP completa en formato raw
    std::string getRawResponse() const;
};

#endif
