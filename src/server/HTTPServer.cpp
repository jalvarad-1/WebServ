#include "HTTPServer.hpp"

HTTPServer::HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg, const ServerConfig & serverConfig):
            _serverConfig(serverConfig)
{
    _socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);

    for (int i = 0; i < 30000; i++) {
        _buffer[i] = 0;
    }
}

ListeningSocket * HTTPServer::get_socket()
{
    return _socket;
}

int HTTPServer::getListeningPort()
{
    return _serverConfig.getPort();
}

void HTTPServer::accepter()
{
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    _new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(_new_socket, _buffer, 30000); // modificar función de lectura por la de leer sockets
}

void HTTPServer::handler()
{
    std::cout << "----------------- Este es el buffer: -----------------" << std::endl;
    std::cout << _buffer << std::endl;
    std::cout << "----------------- Esta es la clase HTTPRequest ----------------" << std::endl;
    std::string raw_request(_buffer);
    HTTPRequest request(raw_request);

    if (request.getErrorMessage() != "") {
        std::cout << "Error: " << request.getErrorMessage() << std::endl;
        return ;
    }

    std::cout << "Method: " << request.getMethod() << std::endl;
    std::cout << "URI: " << request.getURI() << std::endl;
    std::cout << "HTTP Version: " << request.getVersion() << std::endl;

    // Printing headers
    std::map<std::string, std::string> headers = request.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;

    if (request.methodAcceptsBody(request.getMethod()))
        std::cout << "Body: " << request.getBody() << std::endl;

    return ;
}

void HTTPServer::responder()
{
    char body[512];
    sprintf(body, "Hello from server on port %d", getListeningPort());

    char response[1024];
    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body
    );

    write(_new_socket, response, strlen(response));
    close(_new_socket);
}


void HTTPServer::launch()
{
    while(true)
    {
        std::cout << "===== WAITING =====" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << "===== DONE =====" << std::endl;
    }
}
