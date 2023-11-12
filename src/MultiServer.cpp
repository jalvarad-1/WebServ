#include "MultiServer.hpp"

MultiServer::MultiServer(const std::vector<ServerConfig>& serverConfigs) {
    for (std::vector<ServerConfig>:: const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    {
        int port = it->getPort();  // Asumiendo que ServerConfig tiene un método getPort()
        std::cout << port << "before" << std::endl;
        HTTPServer* server = new HTTPServer(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10, *it);
        // std::cout << (*it).getPort() << "class multiserver" << std::endl;
        servers.push_back(server);
        struct pollfd pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd = server->get_socket()->get_sock();  // Asumiendo que get_socket() devuelve un puntero a una clase con el método get_sock()
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
                std::cout << "run ()" << servers[i]->getListeningPort() << std::endl;
                servers[i]->accepter();
                servers[i]->handler();
                servers[i]->responder();
            }
        }
    }
}
