#include "ListeningSocket.hpp"
#include <fcntl.h>

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port,
            u_long interface, int bklg)
{
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = interface;
    _sock = socket(domain, service, protocol);
    testConnection(_sock);
    int reuse = 1;
    testConnection(setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)));
    _binding = bind(_sock, (struct sockaddr *)&_address, sizeof(_address));
    testConnection(_binding);
    _backlog = bklg;
    _listening = listen(_sock, _backlog);
    testConnection(_listening);
    std::cout << "Webserv Info: Socket on port " << port << " ready :)" << std::endl;
}

void ListeningSocket::testConnection(int item_to_test)
{
    if (item_to_test < 0)
    {
		std::cerr << "Webserv Error: Failed to connect" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int ListeningSocket::getBinding()
{
    return _binding;
}

struct sockaddr_in ListeningSocket::getAddress()
{
    return _address;
}

int ListeningSocket::getSock()
{
    return _sock;
}
