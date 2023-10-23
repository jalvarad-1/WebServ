#include "HTTPServer.hpp"

HTTPServer::HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg):
            SimpleServer::SimpleServer(domain, service, protocol,
            port, interface, bklg)
{
    for (int i = 0; i < 30000; i++) {
        buffer[i] = 0;
    }
    _port = port;
}
void HTTPServer::accepter()
{
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(new_socket, buffer, 30000);
}

void HTTPServer::handler()
{
    std::cout << buffer << std::endl;
}

void HTTPServer::responder()
{
    char body[512];
    sprintf(body, "Hello from server on port %d", _port);

    char response[1024];
    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body
    );

    write(new_socket, response, strlen(response));
    close(new_socket);
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
