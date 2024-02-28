
#ifndef CGIMANAGER_H
#define CGIMANAGER_H

# include <iostream>
# include <map>
# include <unistd.h>
# include <sys/socket.h>
# include "../http_request/HTTPRequest.hpp"
# include "routing/Response.hpp"
# include "CGI/CGI.hpp"

# define CGI_BUFFER_SIZE 4200

struct BufferCGI {
    std::string     buffer_str;
	pid_t			pid;
    int				out_socket;
	std::string		in_body_filename;
} ;

class CGIManager {

	public:

		bool readOutput(int fd) ;
		int executeCGI(std::string cgi_pass, std::string binary_path, HTTPRequest & httpRequest, int socket) ;
		void eraseFile(std::string & fileName);

	protected:

	private:
		std::map<int, BufferCGI> _bufferedCGIs ;
		void returnResponse(std::string & responseStr, int outSocket) ;


} ;

#endif
