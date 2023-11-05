#ifndef HTTPServer_hpp
#define HTTPServer_hpp
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include "SimpleServer.hpp"
#include "./http_request/HTTPRequest.hpp"
#include "./ConfigParser/ServerConfig.hpp"

class HTTPServer: public SimpleServer
{
    private:
        char buffer[30000]; // TODO C++11
        int new_socket;
        ServerConfig _serverConfig;

    public:
        HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg, const ServerConfig & serverConfig);
        void launch(); // No se utilzará de forma directa, pero si se requiere para hacer pruebas del http server es posible utilizarla

        void accepter();
        void handler();
        void responder();

        int getListeningPort();
};
#endif
