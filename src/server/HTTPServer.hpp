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
#define NEW_REQUEST 0
#define FILLING_BODY 1
#define CHUNKED_BODY 2
#define FULL_REQUEST 3

struct BufferRequest {
	BufferRequest() ;
    std::string     buffer_str;
    int			content_length;
    HTTPRequest		request;
    short int status;
} ;

class HTTPServer
{
    private:
        char _buffer[30000]; // TODO C++11
        int _new_socket;
        ListeningSocket * _socket;
        ServerConfig _serverConfig;
		std::map<int, BufferRequest> _bufferedRequests ;
		static std::string temp_file_path;
		static int temp_file_counter;

    public:
        HTTPServer(int domain, int service, int protocol,
        int port, u_long interface, int bklg, const ServerConfig & serverConfig);

        int acceptConnection();
        void handler();
        void readPetition(int socket);
        // int sendResponse(int socket, HTTPRequest & request, CGIManager & cgiManager);
		int sendResponse(int socket, Response & httpResponse);
        void checkSock(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status, size_t i);
        ListeningSocket * get_socket();
        int getListeningPort();

		int handleEvent( int socket, CGIManager & cgiManager ) ;
		int handleRead( int socket, BufferRequest & bufferRequest ) ;
		ssize_t readFromFd( int socket, std::string & bufferStr ) ;
		bool parseChunk(std::string & bufferStr, int wr_fd, int * content_length);
		short int getRequestStatus(BufferRequest & bufferRequest);
		int read_content_length_body( int socket, BufferRequest & bufferRequest, std::string & bufferStr );
		int read_chunked_body( int socket, BufferRequest & bufferRequest, std::string & bufferStr );
		std::string get_temp_file();
};
#endif
