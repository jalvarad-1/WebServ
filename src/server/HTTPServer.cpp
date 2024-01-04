#include "HTTPServer.hpp"
#include "MultiServer.hpp"

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

int HTTPServer::acceptConnection()
{
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    // std::cout << "Puerto usado: " << get_socket()->get_sock() << std::endl;
    _new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (_new_socket == -1)
        std::cout << "Error" << std::endl;
    std::cout << "\nRecibimos conexion socket: " << _new_socket << std::endl;
    return (_new_socket);
}

void HTTPServer::readPetition(int socket) {
    recv(socket, _buffer, 30000, MSG_DONTWAIT);
    std::cout << "Leemos peticion" << std::endl;
    // read(poll_fds.fd, _buffer, 30000);
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

std::string readFromFile(std::string filename) {
    std::ifstream file(filename.c_str());
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    } else {
        return "No se pudo abrir el archivo.";
    }
}

void HTTPServer::sendResponse(struct fd_status &status, int socket)
{   
    HTTPRequest request(_buffer);
    // std::string range = request.getHeader("Range");
    std::string root = status.server->_serverConfig.getRoot();
    // std::string file = readFromFile(root + "/index.html");
    
    // std::string body = file;
    char body[512];
    sprintf(body, "Hello from server on port %d and socket %d", getListeningPort(), socket);

    char response[100+strlen(body)];
    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "Accept-Ranges: bytes\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body
    );
    send(socket, response, strlen(response), 0);
    std::cout << "Mandamos respuesta" << std::endl;
    close(socket);
    std::cout << "Cerramos socket: " << socket << std::endl;
}

// void HTTPServer::launch(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status)
// {
//     while(true)
//     {
//         std::cout << "===== WAITING =====" << std::endl;
//         accepter(poll_fds, status);
//         handler();
//         responder();
//         std::cout << "===== DONE =====" << std::endl;
//     }
// }
