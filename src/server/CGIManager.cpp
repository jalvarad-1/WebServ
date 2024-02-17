
#include "CGIManager.hpp"

bool CGIManager::readOutput( int fd ) {
	char buffer[CGI_BUFFER_SIZE] ;
	ssize_t bytes_read ;
	std::cerr << "LLEGO AL READ" << std::endl;
	bytes_read = read(fd, buffer, CGI_BUFFER_SIZE - 1);
	// bytes_read = recv(fd, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
	// std::cerr << bytes_read << " bytes read from fd " << fd << std::endl;
	switch (bytes_read) {
		case -1:
			std::cerr << "CGI READ DA -1" << std::endl;
			// sleep(1);
			return true ;
		case 0:
			std::cerr << "CGI READ DA 0" << std::endl;
			// sleep(1);
			returnResponse(_bufferedCGIs[fd].buffer_str, _bufferedCGIs[fd].out_socket);
            std::cout << "Cerramos el fd: " << fd << std::endl;
            close(fd);
			_bufferedCGIs.erase(fd);
			return false ;
			break ;
		default:
			std::cerr << "CGI READ DA " << bytes_read << std::endl;
			// sleep(1);
			buffer[bytes_read] = '\0';
			// if ( readFromBuffer(_bufferedCGIs[fd], buffer) )
			// 	return true ;
			// sendResponse(fd, _bufferedCGIs[fd].request);
			_bufferedCGIs[fd].buffer_str.append(buffer);
			if (waitpid(_bufferedCGIs[fd].pid, NULL, WNOHANG) != 0) {
				std::cerr << "PROGRAMA MUERTO" << std::endl;
				bytes_read = read(fd, buffer, CGI_BUFFER_SIZE - 1);
				while (bytes_read > 0) {
					_bufferedCGIs[fd].buffer_str.append(buffer);
					bytes_read = read(fd, buffer, CGI_BUFFER_SIZE - 1);
				}
				returnResponse(_bufferedCGIs[fd].buffer_str, _bufferedCGIs[fd].out_socket);
				_bufferedCGIs.erase(fd);
				return false ;
			}
			std::cerr << "PROGRAMA VIVO" << std::endl;
			return true ;
	}
}

void CGIManager::returnResponse(std::string & responseStr, int outSocket) {
	std::cout << "\n---Response---\n" << responseStr <<  "---" << std::endl;
    send(outSocket, responseStr.c_str(), responseStr.size(), 0);
	close(outSocket);
}

int CGIManager::executeCGI(std::string cgi_pass, std::string binary_path, HTTPRequest & httpRequest, int socket) {
	BufferCGI bufferCGI;
	
	std::map<std::string, std::string> env;
    env["REQUEST_METHOD"] = httpRequest.getMethod();
    env["SERVER_PROTOCOL"] = httpRequest.getVersion();
    env["PATH_INFO"] = cgi_pass; // TODO Review the meaning of PATH_INFO
	
	std::vector<std::string> args;
	CGI cgi(cgi_pass);
	args.push_back(binary_path);

	cgi.set_env(env);
	cgi.set_args(args);
	
	// TODO Control de errores
	int outFd = cgi.exec_cgi(httpRequest.getBody(), &bufferCGI.pid);
	bufferCGI.out_socket = socket;
	_bufferedCGIs[outFd] = bufferCGI;
	std::cerr << "--VOY A DEVOLVER " << outFd << std::endl;
	return outFd;
}