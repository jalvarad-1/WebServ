#include "MultiServer.hpp"

MultiServer::MultiServer(const std::vector<int>& ports) {
    for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); ++it)
    {
        int port = *it;
        HTTPServer* server = new HTTPServer(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10);
        servers.push_back(server);

        struct pollfd pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd = server->get_socket()->get_sock();
        pfd.events = POLLIN;
        pfd.revents = 0;
        poll_fds.push_back(pfd);
    }
}
// SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);

MultiServer::~MultiServer() {
    for (std::vector<HTTPServer*>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        HTTPServer* server = *it;
        delete server;
    }
}

void MultiServer::run() {
    while (true) {
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret < 0) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].revents & POLLIN) {
                // TODO keep-alive connections
                servers[i]->accepter();
                servers[i]->handler();
                servers[i]->responder();
            }
        }
    }
}
