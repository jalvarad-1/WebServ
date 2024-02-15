
#include "CGIManager.hpp"

bool CGIManager::readOutput( int fd ) {
	char buffer[CGI_BUFFER_SIZE] ;
	ssize_t bytes_read ;
	bytes_read = read(fd, buffer, CGI_BUFFER_SIZE - 1);
	// bytes_read = recv(fd, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
	// std::cerr << bytes_read << " bytes read from fd " << fd << std::endl;
	switch (bytes_read) {
		case -1:
			return true ;
		case 0:
            std::cout << "Cerramos el fd: " << fd << std::endl;
            close(fd);
			_bufferedCGIs.erase(fd);
			return false ;
			break ;
		default:
			buffer[bytes_read] = '\0';
			// if ( readFromBuffer(_bufferedCGIs[fd], buffer) )
			// 	return true ;
			// sendResponse(fd, _bufferedCGIs[fd].request);
			_bufferedCGIs[fd].buffer_str.append(buffer);
			if (waitpid(_bufferedCGIs[fd].pid, NULL, WNOHANG) != 0) {
				while (bytes_read > 0) {
					_bufferedCGIs[fd].buffer_str.append(buffer);
					bytes_read = read(fd, buffer, CGI_BUFFER_SIZE - 1);
				}
				returnResponse(_bufferedCGIs[fd].buffer_str, _bufferedCGIs[fd].out_socket);
				_bufferedCGIs.erase(fd);
				return false ;
			}
			return true ;
	}
}

void returnResponse(std::string & responseStr, int outSocket) {
    send(outSocket, responseStr.c_str(), responseStr.size(), 0);
}

void executeCGI(...) {
	BufferCGI bufferCGI;
	int outFd = exec(..., &bufferCGI.pid);
}