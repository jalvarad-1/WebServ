#ifndef MultiServer_hpp
#define MultiServer_hpp
#include <stdio.h>
#include "HTTPServer.hpp"
#include <vector>
#include <poll.h>
#include <cstring> // Para memset
#include <chrono>  // Para la hora
#include <cstdlib>
#include "../ConfigParser/ConfigParser.hpp"

struct fd_status{
    std::chrono::system_clock::time_point fecha;
    int status;
    bool port;
    HTTPServer* server;
};

class MultiServer
{
    private:
        // std::vector<HTTPServer*> servers;
        std::vector<struct pollfd> poll_fds;
        std::vector<struct fd_status> status;

    public:
        /// TODO to decide type of variable to pass to Constructor
        std::vector<struct pollfd> get_fds();
        std::vector<struct fd_status> get_status();
        void set_fds(struct pollfd);
        void set_status(struct fd_status);
        MultiServer(const std::vector<ServerConfig>& serverConfigs);
        ~MultiServer();
        void run();
};
#endif
