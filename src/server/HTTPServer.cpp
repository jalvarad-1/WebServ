#include "HTTPServer.hpp"
#include "MultiServer.hpp"
#include "../utils/utils.hpp"
#include "response_code/ResponseCode.hpp"
#include <ctime>
#include "routing/Routing_ns.hpp"

BufferRequest::BufferRequest( void ) {
	content_length = -1;
}

HTTPServer::HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg, const ServerConfig & serverConfig):
            _serverConfig(serverConfig)
{
    contentMap["html"] = "text/html";
    contentMap["htm"] = "text/json";
    contentMap["json"] = "text/json";
    contentMap["xml"] = "text/xml";
    contentMap["jpeg"] = "image/jpeg";
    contentMap["png"] = "image/png";
    contentMap["gif"] = "image/gif";
    contentMap["mp3"] = "audio/mpeg";
    contentMap["wav"] = "audio/wav";
    contentMap["mp4"] = "video/mp4";
    contentMap["mpeg"] = "video/mpeg";
    contentMap["mpg"] = "video/mpeg";
    contentMap["pdf"] = "application/pdf";
    _socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
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

bool HTTPServer::readFromBuffer( BufferRequest & bufferRequest, char * buffer ) {
	bufferRequest.buffer_str.append(buffer);
    std::cout << "INTENTAMOS leer " << std::endl;
	if (bufferRequest.content_length < 0) {
		size_t rnrn = bufferRequest.buffer_str.find("\r\n\r\n");
		if ( rnrn == std::string::npos )
			return true ;
		if ( bufferRequest.request.getMethod().empty() ) {
			bufferRequest.request = HTTPRequest(bufferRequest.buffer_str.substr(0, rnrn + 4));
			std::cout << "\n---Request---\n" << bufferRequest.buffer_str.substr(0, rnrn + 4) << "---" << std::endl;
			bufferRequest.content_length = bufferRequest.request.returnContentLength();
			if ( bufferRequest.content_length < 0 )	
				return false ;
			bufferRequest.buffer_str.erase(bufferRequest.buffer_str.begin() + rnrn + 4);
		}
	}
	if ( static_cast<int>(bufferRequest.buffer_str.size()) >= bufferRequest.content_length ) {
		bufferRequest.request._body = bufferRequest.buffer_str;
		std::cout << "\n---Body---\n" << bufferRequest.buffer_str << "---" << std::endl;
		return false ;
	}
	return true ;
}

bool HTTPServer::readFromFd( int socket ) {
	char buffer[SERVER_BUFFER_SIZE] ;
	ssize_t bytes_read ;
	bytes_read = recv(socket, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
	// std::cerr << bytes_read << " bytes read from socket " << socket << std::endl;
	switch (bytes_read) {
		case -1:
			return true ;
		case 0:
            std::cout << "Cerramos el socket: " << socket << std::endl;
            close(socket);
			_bufferedRequests.erase(socket);
			return false ;
			break ;
		default:
			buffer[bytes_read] = '\0';
			if ( readFromBuffer(_bufferedRequests[socket], buffer) )
				return true ;
			sendResponse(socket, _bufferedRequests[socket].request);
			_bufferedRequests.erase(socket);
			return false ;
	}
}

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

    if (request.methodAcceptsBody())
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

std::string getContentType(std::string file_path, std::map<std::string, std::string> contentMap) {
    std::vector<std::string> path = split_char(file_path, '/');
    std::string file = path.empty()? "" : path.back();
    std::cout << "segunda linea file path" << std::endl;
    std::vector<std::string> file_split = split_char(file, '.');
    std::string extension;
    std::string content_type;
    if (file_split.size() > 1) {
        extension = file_split.back();
        content_type = contentMap[extension];
        if (content_type == "")
            content_type = "text/plain";
        // if (extension == "html" || extension == "htm")
        //     content_type = "text/html";
        // else if (extension == "json")
        //     content_type = "text/json";
        // else if (extension == "xml")
        //     content_type = "text/xml";
        // else if (extension == "jpeg")
        //     content_type = "image/jpeg";
        // else if (extension == "png")
        //     content_type = "image/png";
        // else if (extension == "gif")
        //     content_type = "image/gif";
        // else if (extension == "mp3")
        //     content_type = "audio/mpeg";
        // else if (extension == "wav")
        //     content_type = "audio/wav";
        // else if (extension == "mp4")
        //     content_type = "video/mp4";
        // else if (extension == "mpeg" || extension == "mpg")
        //     content_type = "video/mpeg";
        // else if (extension == "pdf")
        //     content_type = "application/pdf";
        // else
        //     content_type = "text/plain";
    }
    else {
        content_type = "text/plain";
    }
    std::cout << "getContentType: " << content_type << std::endl;
    return (content_type);
}

void HTTPServer::sendResponse(int socket, HTTPRequest & request)
{   
    // Esta clase se encarga de parsear la petición.
    // HTTPRequest request(requestStr);
    std::cout << "Uri: " << request.getURI() << std::endl;
    // En esta clase almacenamos todos los códigos de error y su mensaje.
    ResponseCode response_codes;

    // Clase que simula la respuesta que me va a llegar del rooting
    Response my_response = Routing::returnResource(this->_serverConfig, request);
    //my_response.file_path = "/home/asdas/archivo.html";
    //my_response.string_body = "Este es el mensaje que devolverá la página web!";
    //my_response.response_code = 400;

    std::string date = getDate();

    std::stringstream response;

    response << "HTTP/1.1 " << my_response.response_code << " " << response_codes.get_code_string(my_response.response_code) << "\r\n";
    response << "Content-Type: " << getContentType(my_response.file_path, this->contentMap) << "\r\n";
    // response << "Content-Length: " << my_response.string_body.size() << "\r\n";
	/// SUPER ÑAPA ///
	if ( ñapaCounter > 1 ) {
    	response << "Content-Length: " << "100000000" << "\r\n";
	} else {
    	response << "Content-Length: " << my_response.string_body.size() << "\r\n";
	}
	///
    response << "Connection: close\r\n";
    response << "Date: " << date << "\r\n";
    response << "Location: " << my_response.headers["Location"] << "\r\n";
    // Location header llegará cuando se crea en el routing, con concatenar los headers que llegan
    // del response valdría
    response << "\r\n";
    // response << my_response.string_body;
    
	/// SUPER ÑAPA ///
	if ( ñapaCounter > 1 ) {
		char maldito_char = *(request.getBody().end() - 1) - 32;
		for (int i = 100000000; i > 0; i--)
			response << maldito_char;
	} else {
    	response << my_response.string_body;
	}
	///

    // char body[my_response.string_body.size()];

    // snprintf(body, sizeof(body), "%s", my_response.string_body.c_str());
    
    // char response[500+strlen(body)];

    // std::cout << my_response.string_body << std::endl;
	// std::cerr << date.c_str() << std::endl;
	// std::cerr << my_response.string_body.c_str() << std::endl;
    // std::cout << "sendResponse" << std::endl;
    // sprintf(response,
    //     "HTTP/1.1 %d %s\r\n"
    //     "Content-Type: %s\r\n"
    //     "Content-Length: %zu\r\n"
    //     "Connection: close\r\n"
    //     "Date: %s\r\n"
    //     "\r\n"
    //     "%s",
    //     my_response.response_code,
    //     response_codes.get_code_string(my_response.response_code).c_str(),
    //     getContentType(my_response.file_path).c_str(),
    //     strlen(my_response.string_body.c_str()),
    //     date.c_str(),
    //     my_response.string_body.c_str()
    // );

	std::cout << "\n---Response---\n" << response.str() <<  "---" << std::endl;
    send(socket, response.str().c_str(), response.str().size(), 0);
    std::cout << "Cerramos el socket: " << socket << std::endl;
    close(socket);

}
