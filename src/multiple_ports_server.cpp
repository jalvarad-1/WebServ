/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_ports_server.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalvarad <jalvarad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 16:12:36 by jalvarad          #+#    #+#             */
/*   Updated: 2023/09/23 18:27:21 by jalvarad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <poll.h>

#define SERVER_A "81"
#define SERVER_B "82"
#define SERVER_C "83"

struct Server {
    std::string server_name;
    struct addrinfo* servinfo;
    std::string port;
	int socketfd;
	Server(const std::string& name, struct addrinfo* info, const std::string& p) 
        : server_name(name), servinfo(info), port(p) {}
};

int main(void) {
    struct addrinfo hints, *p;
    int sockfd;
	//int new_fd;
    //struct sockaddr_storage their_addr;
    //socklen_t sin_size;
    //struct sigaction sa;
    int yes = 1;
    int conection;

    // List of servers
    std::vector<Server> servers;
	servers.push_back(Server("NameA", NULL, SERVER_A));
	servers.push_back(Server("NameB", NULL, SERVER_B));
	servers.push_back(Server("NameC", NULL, SERVER_C));

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
        conection = getaddrinfo(NULL, it->port.c_str(), &hints, &(it->servinfo));
        if (conection != 0) {
            std::cout << "getaddrinfo (" << it->server_name << "): " << gai_strerror(conection) << std::endl;
            return 1;
        }

        for (p = it->servinfo; p != NULL; p = p->ai_next) {
            sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (sockfd == -1) {
                std::cout << it->server_name << " error: socket" << std::endl;
                continue;
            }
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                std::cout << "Error: setsockopt" << std::endl;
                exit(1);
            }
            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                std::cout << it->server_name << " error: bind" << std::endl;
                continue;
            }
            break;
        }
        freeaddrinfo(it->servinfo);
        if (p == NULL) {
            std::cout << it->server_name << " error: bind not possible" << std::endl;
            exit(1);
        }
        if (listen(sockfd, SOMAXCONN) == -1) {
            std::cout << it->server_name << " error: listening" << std::endl;
            exit(1);
        }
		it->socketfd = sockfd;
		std::cout << "WebServ: " << it->server_name << "Waiting for new connections..." << std::endl;
    }

    // Falta utilizar ¡
}

