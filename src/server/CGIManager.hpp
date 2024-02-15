
#ifndef CGIMANAGER_H
#define CGIMANAGER_H

# include <iostream>
# include <map>
# include <unistd.h>
# include <sys/socket.h>

# define CGI_BUFFER_SIZE 4200

struct BufferCGI {
    std::string     buffer_str;
	pid_t			pid;
    int				out_socket;
} ;

class CGIManager {

	public:

		bool readOutput(int fd) ;

	protected:

	private:

		std::map<int, BufferCGI> _bufferedCGIs ;
		void returnResponse(std::string & responseStr, int outSocket) ;

} ;

#endif