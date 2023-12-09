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
};

class MultiServer
{
    private:
        std::vector<HTTPServer*> servers;
        std::vector<struct pollfd> poll_fds;
        std::vector<struct fd_status> status;

    public:
        /// TODO to decide type of variable to pass to Constructor
        MultiServer(const std::vector<ServerConfig>& serverConfigs);
        ~MultiServer();
        void run();
};
#endif
