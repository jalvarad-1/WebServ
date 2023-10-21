#ifndef MultiServer_hpp
#define MultiServer_hpp
#include <stdio.h>
#include "HTTPServer.hpp"
#include <vector>
#include <poll.h>
#include <cstring> // Para memset

class MultiServer
{
    private:
        std::vector<HTTPServer*> servers;
        std::vector<struct pollfd> poll_fds;

    public:
        /// TODO to decide type of variable to pass to Constructor
        MultiServer(const std::vector<int>& ports);
        ~MultiServer();
        void run();
};
#endif
