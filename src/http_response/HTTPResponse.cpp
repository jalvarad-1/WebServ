#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse(const ServerConfig &server_config, const HTTPRequest &request, int port) {
    ;
}
// /kapouet    /kapouet/pouic/toto/pouet
LocationRules ServerConfig::findBestMatchingLocation(const std::string &uri) const
{
    //Revisar si es un directorio y en caso contrario devolver algo por defecto
    size_t maxMatchLength = 0;
    std::map<std::string, LocationRules>::const_iterator bestMatch = locations.end();

    for (std::map<std::string, LocationRules>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        const std::string &locationPrefix = it->first;

        if (locationPrefix.size() > maxMatchLength && locationPrefix.size() <= uri.size() &&
            std::equal(locationPrefix.begin(), locationPrefix.end(), uri.begin())) {
            bestMatch = it;
            maxMatchLength = locationPrefix.size();
        }
    }

    if (bestMatch != locations.end()) {
        return bestMatch->second;
    }

    //Manejar caso donde no hay ninguna coincidencia.
}
//Pasos viables:

//Una vez tenemos el server del que tirar cogemos la ruta a devolver, revisando que el método de la petición sea el permitid
//y si no existe el recurso devolver 404

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
