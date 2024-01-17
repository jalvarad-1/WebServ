#include "MultiServer.hpp"

MultiServer::MultiServer(const std::vector<ServerConfig>& serverConfigs) {
    for (std::vector<ServerConfig>:: const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    {
        int port = it->getPort();  // Asumiendo que ServerConfig tiene un método getPort()
        struct fd_status port_status;
        port_status.server = new HTTPServer(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 0, *it);
        struct pollfd pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd = port_status.server->get_socket()->get_sock();  // Asumiendo que get_socket() devuelve un puntero a una clase con el método get_sock()
        pfd.events = POLLIN;
        pfd.revents = 0;
        port_status.port = true;
        port_status.status = 0;
        poll_fds.push_back(pfd);
        status.push_back(port_status);
    }
}

MultiServer::~MultiServer() {
    for (std::vector<fd_status>::iterator it = status.begin(); it != status.end(); ++it)
    {
        HTTPServer* server = it->server;
        delete server;
    }
}

void MultiServer::run() {
    int socket;
    while (true) {
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret < 0) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].revents & POLLIN) {
                // std::cout << "run ()" << status[i].server->getListeningPort() << std::endl;
                // if (status[i].port == true) {
                socket = status[i].server->acceptConnection();
                status[i].server->readPetition(socket);
                status[i].server->sendResponse(socket);
                // }
            }
        }
    }
}

std::vector<struct pollfd> MultiServer::get_fds() {
    return poll_fds;
}
std::vector<struct fd_status> MultiServer::get_status() {
    return status;
}
void MultiServer::set_fds(struct pollfd new_poll_fd) {
    poll_fds.push_back(new_poll_fd);
}
void MultiServer::set_status(struct fd_status new_status) {
    status.push_back(new_status);
}  

