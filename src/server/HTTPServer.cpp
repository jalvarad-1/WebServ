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

void HTTPServer::sendResponse(struct pollfd &poll_fds, struct fd_status &status)
{
    char body[512];
    if (status.status == 1)
        sprintf(body, "Hello from server on port %d and socket %d", getListeningPort(), poll_fds.fd);
    else
        sprintf(body, "Hello from server on port %d not closed", getListeningPort());
    // std::cout << status.status << std::endl;
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

    send(poll_fds.fd, response, strlen(response), 0);
    // write(poll_fds.fd, response, strlen(response));
    // close(_new_socket);
}

void HTTPServer::checkConnection(struct pollfd &poll_fds, struct fd_status &status) {
    if (status.status > 4 && status.port == false) {
        close(poll_fds.fd);
        status.status = -1;
        // std::cout << "Cierro!!" << std::endl;
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
