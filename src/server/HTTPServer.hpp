#ifndef HTTPServer_hpp
#define HTTPServer_hpp
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include "../http_request/HTTPRequest.hpp"
#include "../ConfigParser/ServerConfig.hpp"
#include "ListeningSocket.hpp"
#include <poll.h>

class HTTPServer
{
    private:
        char _buffer[30000]; // TODO C++11
        int _new_socket;
        ListeningSocket * _socket;
        ServerConfig _serverConfig;

    public:
        HTTPServer(int domain, int service, int protocol,
        int port, u_long interface, int bklg, const ServerConfig & serverConfig);
        // void launch(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status); // No se utilzará de forma directa, pero si se requiere para hacer pruebas del http server es posible utilizarla

        void acceptConnection(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status);
        void handler();
        void checkConnection(struct pollfd &poll_fds, struct fd_status &status);
        void readPetition(struct pollfd &poll_fds);
        void sendResponse(struct pollfd &poll_fds, struct fd_status &status);
        void checkSock(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status, size_t i);
        ListeningSocket * get_socket();
        int getListeningPort();
};
#endif
