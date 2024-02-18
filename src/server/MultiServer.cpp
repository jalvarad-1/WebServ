#include "MultiServer.hpp"

MultiServer::MultiServer(const std::vector<ServerConfig>& serverConfigs) {
    for (std::vector<ServerConfig>:: const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    {
        int port = it->getPort();
        u_long interface = it->getHost();

        struct fd_status port_status;
        port_status.server = new HTTPServer(AF_INET, SOCK_STREAM, interface, port, INADDR_ANY, 0, *it);
		waifu.push_back(port_status.server);
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
    int socket, listeningFds = poll_fds.size(), serverSockets = listeningFds, readResult;
	CGIManager cgiManager;
    while (true) {
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret < 0) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }
        for (int i = static_cast<int>(poll_fds.size()) - 1; i >= 0 ; i--) {
			if (poll_fds[i].revents & (POLLIN|POLLHUP)) {

                // std::cout << "run ()" << status[i].server->getListeningPort() << std::endl;
                // if (status[i].port == true) {
				if ( i < listeningFds ) {
					std::cerr << "RECIBIMOS DATOS POR EL listening FD " << poll_fds[i].fd << std::endl;
					socket = waifu[i]->acceptConnection();
                    //status[i].server->readPetition(socket);
					struct pollfd pfd;
					memset(&pfd, 0, sizeof(pfd));
					pfd.fd = socket;  // Asumiendo que get_socket() devuelve un puntero a una clase con el método get_sock()
					pfd.events = POLLIN;
					pfd.revents = 0;
					poll_fds.push_back(pfd);
					waifu.push_back(waifu[i]);
					status.push_back(status[i]);
					serverSockets++;
					// status[i].server->addActiveFd(socket);
				} else if ( i < serverSockets ) {
					std::cerr << "RECIBIMOS DATOS POR EL socket FD " << poll_fds[i].fd << std::endl;
					readResult = waifu[i]->handleEvent(poll_fds[i].fd, cgiManager);
					switch (readResult) {
						case -1:
							continue ;
						case 0:
							poll_fds.erase(poll_fds.begin() + i);
							waifu.erase(waifu.begin() + i);
							serverSockets--;
							break ;
						default:
							struct pollfd pfd;
							memset(&pfd, 0, sizeof(pfd));
							pfd.fd = readResult;  // Asumiendo que get_socket() devuelve un puntero a una clase con el método get_sock()
							pfd.events = POLLIN;
							pfd.revents = 0;
							std::cerr << "VOY A AÑADIR EL FD " << readResult << std::endl;
							// std::cerr << "POLL_FDS TIENE UN SIZE DE " << poll_fds.size() << std::endl;
							poll_fds.push_back(pfd);
							std::cerr << "VOY A BORRAR EL FD " << poll_fds[i].fd << std::endl;
							// sleep(5);
							poll_fds.erase(poll_fds.begin() + i);
							waifu.erase(waifu.begin() + i);
							serverSockets--;
					}
					// if ( !waifu[i]->readFromFd(poll_fds[i].fd) ) {

					// 	poll_fds.erase(poll_fds.begin() + i);
					// 	waifu.erase(waifu.begin() + i);
					// 	serverSockets--;
					// }
				} else {
					std::cerr << "RECIBIMOS DATOS POR EL cgi FD " << poll_fds[i].fd << std::endl;
                    sleep (1);
					if (!cgiManager.readOutput(poll_fds[i].fd)) {
						poll_fds.erase(poll_fds.begin() + i);						
					}
				}
                // status[i].server->readPetition(socket);
                // status[i].server->sendResponse(socket);
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

