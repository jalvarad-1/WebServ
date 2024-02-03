#include "HTTPServer.hpp"
#include "MultiServer.hpp"
#include "../utils/utils.hpp"
#include "response_code/ResponseCode.hpp"
#include <ctime>
#include "routing/Routing_ns.hpp"

HTTPServer::HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg, const ServerConfig & serverConfig):
            _serverConfig(serverConfig)
{
    _socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
	// _requestStrs = std::map<int, std::string *>();
	std::cerr << "size of map: " << _requestStrs.size() << std::endl;
    for (int i = 0; i < 30000; i++) {
        _buffer[i] = 0;
    }
}


ListeningSocket * HTTPServer::get_socket()
{
    return _socket;
}

int HTTPServer::getListeningPort()
{
    return _serverConfig.getPort();
}

int HTTPServer::acceptConnection()
{
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    // std::cout << "Puerto usado: " << get_socket()->get_sock() << std::endl;
    _new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (_new_socket == -1)
        std::cout << "Error" << std::endl;
    std::cout << "\nRecibimos conexion socket: " << _new_socket << std::endl;
    return (_new_socket);
}

// void HTTPServer::readPetition(int socket) {
//     recv(socket, _buffer, 30000, MSG_DONTWAIT);
//     std::cout << "Leemos peticion" << std::endl;
//     std::string pepe(_buffer);
//     std::cout << "resquest :\n" << pepe << std::endl;    // read(poll_fds.fd, _buffer, 30000);
// }

void HTTPServer::addActiveFd( int newFd ) {
	this->_activeFds.push_back(newFd);
}

// void HTTPServer::readFromFds( void ) {
// 	char buffer[SERVER_BUFFER_SIZE];
// 	ssize_t bytes_read;
// 	std::string * requestStr;
// 	for ( std::list<int>::iterator iter = _activeFds.begin(); iter != _activeFds.end(); iter++ ) {
// 		bytes_read = recv(*iter, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
// 		std::cerr << bytes_read << " bytes read from socket " << *iter;
// 		switch (bytes_read) {
// 			case -1:
// 				continue ;
// 			case 0:
// 				if ( _requestStrs[*iter] != NULL )
// 					delete _requestStrs[*iter] ;
// 				_activeFds.remove(*iter) ;
// 				break ;
// 			default:
// 				buffer[bytes_read] = '\0';
// 				std::cout << "BUFFER:\n" << std::string(buffer) << "\n\n" << std::endl;
// 				requestStr = _requestStrs[*iter];
// 				if ( requestStr == NULL ) {
// 					requestStr = new std::string();
// 					_requestStrs[*iter] = requestStr ;
// 				}
// 				requestStr->append(buffer);
// 				std::cout << "resquest :\n<<" << *requestStr << ">>" << std::endl;    // read(poll_fds.fd, _buffer, 30000);
// 				if ( requestStr->size() > 3 && requestStr->find("\r\n\r\n", requestStr->size() - 4) != std::string::npos ) {
// 					std::cout << "YAY! I FOUND THE END! (￣▽￣)" << std::endl;
// 					std::cout << "Leemos peticion" << std::endl;
// 					std::cout << "resquest :\n<<" << *requestStr << ">>" << std::endl;    // read(poll_fds.fd, _buffer, 30000);
// 					delete requestStr;
// 					_activeFds.remove(*iter) ;
// 					requestStr = NULL;
// 				}
// 		}
// 	}
// }

bool HTTPServer::readFromFd( int socket ) {
	char buffer[SERVER_BUFFER_SIZE] ;
	ssize_t bytes_read ;
	bytes_read = recv(socket, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
	std::cerr << bytes_read << " bytes read from socket " << socket << std::endl;
	switch (bytes_read) {
		case -1:
			return true ;
		case 0:
			_requestStrs.erase(socket);
			return false ;
			break ;
		default:
			buffer[bytes_read] = '\0';
			_requestStrs[socket].append(buffer);
			std::cout << "resquest :\n<<" << _requestStrs[socket] << ">>" << std::endl;    // read(poll_fds.fd, _buffer, 30000);
			if ( _requestStrs[socket].size() > 3 && _requestStrs[socket].find("\r\n\r\n", _requestStrs[socket].size() - 4) != std::string::npos ) {
				std::cout << "YAY! I FOUND THE END! (￣▽￣)" << std::endl;
				std::cout << "Leemos peticion" << std::endl;
				std::cout << "resquest :\n<<" << _requestStrs[socket] << ">>" << std::endl;    // read(poll_fds.fd, _buffer, 30000);
				sendResponse(socket, _requestStrs[socket]);
				std::cerr << "1" << std::endl;
				_requestStrs.erase(socket);
				std::cerr << "2" << std::endl;
				return false ;
			}
			return true ;
	}
}

// void HTTPServer::readPetition(int socket) {
// 	char buffer[SERVER_BUFFER_SIZE];
// 	if (_requestStr == NULL)
// 		_requestStr = new std::string();
// 	// while ( recv(socket, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT) > 0 ) {
// 	// 	buffer[SERVER_BUFFER_SIZE - 1] = '\0';
// 	// 	requestStr->append(buffer);
// 	// }
// 	ssize_t bytes_read = recv(socket, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
// 	std::cout << bytes_read << " bytes read" << std::endl;
// 	// while ( bytes_read > 0 ) {
// 	// for (int i = 0; i < 100; i++) {
// 		if (bytes_read > 0) {
// 			buffer[bytes_read] = '\0';
// 			std::cout << "BUFFER:\n" << std::string(buffer) << "\n\n" << std::endl;
// 			_requestStr->append(buffer);
// 			std::cout << "resquest :\n<<" << *_requestStr << ">>" << std::endl;    // read(poll_fds.fd, _buffer, 30000);
//             if ( _requestStr->size() > 3 && _requestStr->find("\r\n\r\n", _requestStr->size() - 4) != std::string::npos ) {
//                 std::cout << "YAY! I FOUND THE END! (￣▽￣)" << std::endl;
// 				std::cout << "Leemos peticion" << std::endl;
// 				std::cout << "resquest :\n<<" << *_requestStr << ">>" << std::endl;    // read(poll_fds.fd, _buffer, 30000);
// 				delete _requestStr;
// 				_requestStr = NULL;
// 			}
// 		}
// 		// usleep(1000);
// 		// bytes_read = recv(socket, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
// 		// std::cout << bytes_read << " bytes read" << std::endl;
// 	// }
// }

void HTTPServer::handler()
{
    std::cout << "----------------- Este es el buffer: -----------------" << std::endl;
    std::cout << _buffer << std::endl;
    std::cout << "----------------- Esta es la clase HTTPRequest ----------------" << std::endl;
    std::string raw_request(_buffer);
    HTTPRequest request(raw_request);

    if (request.getErrorMessage() != "") {
        std::cout << "Error: " << request.getErrorMessage() << std::endl;
        return ;
    }

    std::cout << "Method: " << request.getMethod() << std::endl;
    std::cout << "URI: " << request.getURI() << std::endl;
    std::cout << "HTTP Version: " << request.getVersion() << std::endl;

    // Printing headers
    std::map<std::string, std::string> headers = request.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;

    if (request.methodAcceptsBody(request.getMethod()))
        std::cout << "Body: " << request.getBody() << std::endl;

    return ;
}

std::string getDate() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return buffer;
}

std::string getContentType(std::string file_path) {
    std::vector<std::string> path = split_char(file_path, '/');
    std::string file = path.empty()? "" : path.back();
    std::cout << "segunda linea file path" << std::endl;
    std::vector<std::string> file_split = split_char(file, '.');
    std::string extension;
    std::string content_type;
    if (file_split.size() > 1) {
        extension = file_split.back();
        if (extension == "html" || extension == "htm")
            content_type = "text/html";
        else if (extension == "json")
            content_type = "text/json";
        else if (extension == "xml")
            content_type = "text/xml";
        else if (extension == "jpeg")
            content_type = "image/jpeg";
        else if (extension == "png")
            content_type = "image/png";
        else if (extension == "gif")
            content_type = "image/gif";
        else if (extension == "mp3")
            content_type = "audio/mpeg";
        else if (extension == "wav")
            content_type = "audio/wav";
        else if (extension == "mp4")
            content_type = "video/mp4";
        else if (extension == "mpeg" || extension == "mpg")
            content_type = "video/mpeg";
        else if (extension == "pdf")
            content_type = "application/pdf";
        else
            content_type = "text/plain";
    }
    else {
        content_type = "text/plain";
    }
    std::cout << "getContentType: " << content_type << std::endl;
    return (content_type);
}

void HTTPServer::sendResponse(int socket, std::string & requestStr)
{   
    // Esta clase se encarga de parsear la petición.
    HTTPRequest request(requestStr);
    std::cout << "Uri: " << request.getURI() << std::endl;
    // En esta clase almacenamos todos los códigos de error y su mensaje.
    ResponseCode response_codes;

    // Clase que simula la respuesta que me va a llegar del rooting
    Response my_response = Routing::returnResource(this->_serverConfig, request);
    //my_response.file_path = "/home/asdas/archivo.html";
    //my_response.string_body = "Este es el mensaje que devolverá la página web!";
    //my_response.response_code = 400;

    char body[my_response.string_body.size()];

    snprintf(body, sizeof(body), "%s", my_response.string_body.c_str());
    
    char response[100+strlen(body)];

    std::string date = getDate();
    std::cout << my_response.string_body << std::endl;
	std::cerr << date.c_str() << std::endl;
	std::cerr << "AAAAAAAAH" << std::endl;
    std::cout << "sendResponse" << std::endl;
    sprintf(response,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "Date: %s\r\n"
        "\r\n"
        "%s",
        my_response.response_code,
        response_codes.get_code_string(my_response.response_code).c_str(),
        getContentType(my_response.file_path).c_str(),
        strlen(my_response.string_body.c_str()),
        date.c_str(),
        my_response.string_body.c_str()
    );
	std::cout << "\n\n" << response <<  "\n\n" << std::endl;
    send(socket, response, strlen(response), 0);
    close(socket);
}

// void HTTPServer::launch(std::vector<struct pollfd> &poll_fds, std::vector<struct fd_status> &status)
// {
//     while(true)
//     {
//         std::cout << "===== WAITING =====" << std::endl;
//         accepter(poll_fds, status);
//         handler();
//         responder();
//         std::cout << "===== DONE =====" << std::endl;
//     }
// }