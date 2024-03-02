#ifndef MultiServer_hpp
#define MultiServer_hpp
#include <stdio.h>
#include "HTTPServer.hpp"
#include <vector>
#include <poll.h>
#include <cstdlib>
#include "../ConfigParser/Parser.hpp"
#include "CGIManager.hpp"

#define LISTENING_PORT 0
#define CONNECTION_SOCKET 1
#define CGI_FD 2

struct fd_info{
    short int fd_type;
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
        std::vector<struct pollfd> getFds();
        void setFds(struct pollfd);
        MultiServer(const std::vector<ServerConfig>& serverConfigs);
        ~MultiServer();
        void run();
        struct pollfd createPollfd(int fd);
        void erasePollfd(int i);
};
#endif
