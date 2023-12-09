#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port,
            u_long interface, int bklg)
{
    // Basic socket creation
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    
    _sock = socket(domain, service, protocol);
    test_connection(_sock);
    /// TODO Review this variable in future
    int reuse =1;
    test_connection(setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)));

    // Bind (connect to network)
    _binding = bind(_sock, (struct sockaddr *)&_address, sizeof(_address));
    test_connection(_binding);

    // start listening
    _backlog = bklg;
    _listening = listen(_sock, _backlog);
    test_connection(_listening);
    std::cout << "socket on port " << port << "ready" << std::endl;
}

void ListeningSocket::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}

int ListeningSocket::get_binding()
{
    return _binding;
}

struct sockaddr_in ListeningSocket::get_address()
{
    return _address;
}

int ListeningSocket::get_sock()
{
    return _sock;
}
