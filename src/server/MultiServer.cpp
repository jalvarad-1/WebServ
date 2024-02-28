#include "MultiServer.hpp"

MultiServer::MultiServer(const std::vector<ServerConfig>& serverConfigs) {
    for (std::vector<ServerConfig>:: const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    {
        int port = it->getPort();
        u_long interface = it->getHost();
		struct fd_info index_entry;
		index_entry.server = new HTTPServer(AF_INET, SOCK_STREAM, interface, port, INADDR_ANY, 0, *it);
		index_entry.fd_type = LISTENING_PORT;
        struct pollfd pfd = create_pollfd(index_entry.server->get_socket()->get_sock());
		fd_index[pfd.fd] = index_entry;
        poll_fds.push_back(pfd);
    }
}

MultiServer::~MultiServer() {
    for (std::map<int,fd_info>::iterator it = fd_index.begin(); it != fd_index.end(); ++it) {
		if (it->second.fd_type == LISTENING_PORT) {
			delete it->second.server ;
		}
	}
}

void MultiServer::run() {
    int socket, readResult;
	CGIManager cgiManager;
    while (true) {
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret < 0) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }
        for (int i = static_cast<int>(poll_fds.size()) - 1; i >= 0 ; i--) {
			if (poll_fds[i].revents & (POLLIN|POLLHUP)) {
				struct fd_info & current_fd = fd_index[poll_fds[i].fd];
				switch (current_fd.fd_type) {
					case LISTENING_PORT:
						std::cerr << "RECIBIMOS DATOS POR EL listening FD " << poll_fds[i].fd << std::endl;
						socket = current_fd.server->acceptConnection();
						poll_fds.push_back(create_pollfd(socket));
						fd_index[socket] = fd_info(CONNECTION_SOCKET, current_fd.server);
						continue ;
					case CONNECTION_SOCKET:
						std::cerr << "RECIBIMOS DATOS POR EL socket FD " << poll_fds[i].fd << std::endl;
						readResult = current_fd.server->handleEvent(poll_fds[i].fd, cgiManager);//cgi -> NULL , socket -> httpServer (map <int , httpServer>)
						switch (readResult) {
							case -1:
								continue ;
							case 0:
								std::cerr << "VOY A BORRAR EL socket FD " << poll_fds[i].fd << std::endl;
								erase_pollfd(i);
								continue ;
							default:
								fd_index[readResult] = fd_info(CGI_FD, NULL);
								poll_fds.push_back(create_pollfd(readResult));
								std::cerr << "VOY A BORRAR EL socket FD " << poll_fds[i].fd << " TRAS EJECUTAR UN CGI" << std::endl;
								erase_pollfd(i);
						}
						continue ;
					case CGI_FD:
						// std::cerr << "RECIBIMOS DATOS POR EL cgi FD " << poll_fds[i].fd << std::endl;
						if (!cgiManager.readOutput(poll_fds[i].fd)) {
							std::cerr << "VOY A BORRAR EL cgi FD " << poll_fds[i].fd << std::endl;
							erase_pollfd(i);						
						}
						continue ;
					default:
						std::cerr << "AYUDA ESTO NO DEBERÍA OCURRIR" << std::endl;
				}
            }		
        }
    }
}

std::vector<struct pollfd> MultiServer::get_fds() {
    return poll_fds;
}

void MultiServer::set_fds(struct pollfd new_poll_fd) {
    poll_fds.push_back(new_poll_fd);
}

struct pollfd MultiServer::create_pollfd(int fd) {
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	return pfd;
}

void MultiServer::erase_pollfd(int i) {
	fd_index.erase(poll_fds[i].fd);
	poll_fds.erase(poll_fds.begin() + i);
}