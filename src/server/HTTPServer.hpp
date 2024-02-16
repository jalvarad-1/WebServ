#ifndef HTTPServer_hpp
#define HTTPServer_hpp
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include "../http_request/HTTPRequest.hpp"
#include "../ConfigParser/ServerConfig.hpp"
#include "ListeningSocket.hpp"
#include <poll.h>
#include "CGIManager.hpp"

#define SERVER_BUFFER_SIZE 4200

struct BufferRequest {
	BufferRequest() ;
    std::string     buffer_str;
    int			content_length;
    HTTPRequest		request;
} ;

class HTTPServer
{
    private:
        char _buffer[30000]; // TODO C++11
        int _new_socket;
        ListeningSocket * _socket;
        ServerConfig _serverConfig;
		
		std::map<int, BufferRequest> _bufferedRequests ;

    public:
        HTTPServer(int domain, int service, int protocol,
        int port, u_long interface, int bklg, const ServerConfig & serverConfig);

        int acceptConnection();
        void handler();
        void readPetition(int socket);
        int sendResponse(int socket, HTTPRequest & request, CGIManager & cgiManager);
        void checkSock(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status, size_t i);
        ListeningSocket * get_socket();
        int getListeningPort();

		bool readFromBuffer( BufferRequest & bufferRequest, char * buffer ) ;
		int readFromFd( int socket, CGIManager & cgiManager ) ;
};
#endif
