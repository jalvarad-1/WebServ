#ifndef MultiServer_hpp
#define MultiServer_hpp
#include <stdio.h>
#include "HTTPServer.hpp"
#include <vector>
#include <poll.h>
#include <cstring> // Para memset
// #include <chrono>  // Para la hora
#include <cstdlib>
#include "../ConfigParser/Parser.hpp"
#include "CGIManager.hpp"

#define LISTENING_PORT 0
#define CONNECTION_SOCKET 1
#define CGI_FD 2

struct fd_info{// 
    short int fd_type;//listening socket / connection  / cgi_fd
    HTTPServer* server;
    fd_info(){};
    fd_info(short int fd_type, HTTPServer* server) : fd_type(fd_type), server(server) {}
};

class MultiServer
{
    private:
        std::vector<struct pollfd> poll_fds;
		std::map<int,fd_info> fd_index;

    public:
        std::vector<struct pollfd> get_fds();
        void set_fds(struct pollfd);
        MultiServer(const std::vector<ServerConfig>& serverConfigs);
        ~MultiServer();
        void run();
        struct pollfd create_pollfd(int fd);
        void erase_pollfd(int i);
};
#endif
