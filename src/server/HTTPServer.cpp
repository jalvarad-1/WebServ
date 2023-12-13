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

void HTTPServer::acceptConnection(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status)
{
    struct fd_status port_status;
    struct pollfd pfd;
    memset(&pfd, 0, sizeof(pfd));
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    // _new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    pfd.fd = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    pfd.events = POLLIN;
    pfd.revents = 0;
    port_status.port = false;
    port_status.status = 1;
    port_status.server = this;
    poll_fds.push_back(pfd);
    status.push_back(port_status);
}

void HTTPServer::readPetition(struct pollfd &poll_fds) {
    recv(poll_fds.fd, _buffer, 30000, 0);
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
    std::ifstream file(filename);
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    } else {
        return "No se pudo abrir el archivo.";
    }
}

void HTTPServer::sendResponse(struct pollfd &poll_fds, struct fd_status &status)
{   
    HTTPRequest request(_buffer);
    std::string range = request.getHeader("Range");
    std::string root = status.server->_serverConfig.getRoot();
    std::string file = readFromFile(root + "/index.html");
    
    std::string body = file;

    // char body[512];
    // if (status.status == 1)
    //     sprintf(body, "Hello from server on port %d and socket %d", getListeningPort(), poll_fds.fd);
    // else
    //     sprintf(body, "Hello from server on port %d not closed", getListeningPort());
    // std::cout << status.status << std::endl;

    char response[100+strlen(body.c_str())];
    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "Connection: keep-alive\r\n"
        "Accept-Ranges: bytes\r\n"
        "\r\n"
        "%s",
        strlen(body.c_str()),
        body.c_str()
    );
    send(poll_fds.fd, response, strlen(response), 0);
}

void HTTPServer::checkConnection(struct pollfd &poll_fds, struct fd_status &status) {
    if (status.status > 4 && status.port == false) {
        close(poll_fds.fd);
        status.status = -1;
    }
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
