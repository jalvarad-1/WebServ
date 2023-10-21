#include "SimpleSocket.hpp"

SimpleSocket::SimpleSocket(int domain, int service, int protocol,
    int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    
    sock = socket(domain, service, protocol);
    test_connection(sock);
    /// TODO Review this variable in future
    int reuse =1;
    test_connection(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)));
}

void SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in SimpleSocket::get_address()
{
    return address;
}

int SimpleSocket::get_sock()
{
    return sock;
}
