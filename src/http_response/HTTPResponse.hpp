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

public:
    // Constructor que toma la configuración del servidor y la solicitud HTTP
    // La conf del server podría ser un elemento privado de HTTPServer
    // de esa manera sabemos que llegará el adecuado
    HTTPResponse(const ServerConfig &server_config, const HTTPRequest &request, int port);

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
