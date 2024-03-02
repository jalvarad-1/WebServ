#include "HTTPServer.hpp"
#include "MultiServer.hpp"
#include "../utils/utils.hpp"
#include "response_code/ResponseCode.hpp"
#include <ctime>
#include "routing/Routing_ns.hpp"

int HTTPServer::temp_file_counter = 0;

std::string HTTPServer::temp_file_path = "./.cache/WAIFU";

BufferRequest::BufferRequest( void ) {
	content_length = 0;
	status = NEW_REQUEST;
}

HTTPServer::HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg, const ServerConfig & serverConfig):
            _serverConfig(serverConfig)
{
    _socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
}

ListeningSocket * HTTPServer::getSocket()
{
    return _socket;
}

int HTTPServer::getListeningPort()
{
    return _serverConfig.getPort();
}

int HTTPServer::acceptConnection()
{
    struct sockaddr_in address = getSocket()->getAddress();
    int addrlen = sizeof(address);
    _new_socket = accept(getSocket()->getSock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (_new_socket == -1)
        std::cout << "Error" << std::endl;
    std::cout << "\nRecibimos conexion socket: " << _new_socket << std::endl;
    return (_new_socket);
}

short int HTTPServer::getRequestStatus(BufferRequest & bufferRequest) {
	std::cerr << "Voy a comprobar el tipo de body en la request" << std::endl;
	if (bufferRequest.request.getHeader("Transfer-Encoding") == "chunked")
		return CHUNKED_BODY;
	bufferRequest.content_length = bufferRequest.request.returnContentLength();
	std::cerr << bufferRequest.request.returnContentLength() << std::endl;
	if ( bufferRequest.content_length != -1 )	
		return FILLING_BODY;
	return FULL_REQUEST;
}

ssize_t HTTPServer::readFromFd( int socket, std::string & bufferStr ) {
	char buffer[SERVER_BUFFER_SIZE] ;
	ssize_t bytes_read = recv(socket, buffer, SERVER_BUFFER_SIZE - 1, MSG_DONTWAIT);
	std::cerr << bytes_read << " read from socket " << socket << std::endl;
	if (bytes_read > 0) {
		buffer[bytes_read] = '\0';
		bufferStr.append(buffer);
	}
	return bytes_read;
}

bool HTTPServer::parseChunk(std::string & bufferStr, int wr_fd, int * content_length) {
	size_t firstRN = bufferStr.find("\r\n");
	if (firstRN == std::string::npos)
		return false ;
	char *pEnd;
	std::string auxStr = bufferStr.substr(0, firstRN);
	double characterN;
	characterN = static_cast<double>(std::strtol(auxStr.c_str(), &pEnd, 16));
	if (characterN == 0)
		return true ;
	firstRN += 2;
	size_t secondRN = bufferStr.find("\r\n", firstRN);
	if (secondRN == std::string::npos)
		return false ;
	auxStr = bufferStr.substr(firstRN, secondRN - firstRN);
	int writtenBytes = write(wr_fd, auxStr.c_str(), auxStr.size());
	if ( writtenBytes != static_cast<int>(auxStr.size())) {
		throw std::runtime_error("Write error");
	}
	*content_length += writtenBytes;
	secondRN += 2;
	bufferStr.erase(0, secondRN);
	return parseChunk(bufferStr, wr_fd, content_length) ;
}

std::string HTTPServer::getTempFile() {
	std::stringstream ret;
	ret << temp_file_path << "_" << temp_file_counter++;
	return ret.str();
}

int HTTPServer::readContentLengthBody( BufferRequest & bufferRequest, std::string & bufferStr ) {
	if (write(bufferRequest.request._body_file_fd, bufferStr.c_str(), bufferStr.size()) != static_cast<int>(bufferStr.size())) {
		throw std::runtime_error("Write error");
	}
	bufferRequest.content_length -= bufferStr.size();
	bufferStr.clear();
	if (bufferRequest.content_length <= 0) {
		close(bufferRequest.request._body_file_fd);
		return 1;
	}
	return -1 ;
}

int HTTPServer::readChunkedBody( BufferRequest & bufferRequest, std::string & bufferStr ) {
	if (parseChunk(bufferStr, bufferRequest.request._body_file_fd, &bufferRequest.content_length))  {
		close(bufferRequest.request._body_file_fd);
		return 1 ;
	}
	return -1 ;
}

int HTTPServer::handleRead( int socket, BufferRequest & bufferRequest ) {
	std::string & bufferStr = bufferRequest.buffer_str;
	ssize_t bytes_read = readFromFd(socket, bufferStr);
	if (bytes_read > 0) {
		size_t rnrn;
		LocationRules locationRules;
		switch (bufferRequest.status) {
			case NEW_REQUEST:
				// BUSCAMOS RNRN Y GENERAMOS LA NUEVA REQUEST
				rnrn = bufferStr.find("\r\n\r\n");
				if ( rnrn == std::string::npos ) {
					std::cerr << "NO HE ENCONTRADO EL FINAL" << std::endl;
					return -1 ;
				}
				rnrn += 4;
				bufferRequest.request = HTTPRequest(bufferStr.substr(0, rnrn), _serverConfig);
				std::cout << "\n---Request---\n" << bufferStr.substr(0, rnrn) << "---" << std::endl;
				
				// CLEAN BUFFER
				bufferStr.erase(0, rnrn);

				// CHECK REQUEST ERRORS
				if (bufferRequest.request.getErrorCode() != 200) {
					Response httpResponse;
					LocationRules & locationRules = *(bufferRequest.request.getLocationRules());
					httpResponse.response_code = bufferRequest.request.getErrorCode();
					if (httpResponse.response_code == 302) {
						httpResponse.headers["Location"] = locationRules.getRedirect();
					} else {
						Routing::errorResponse(httpResponse, locationRules);
					}
					sendResponse(socket, httpResponse);
					return 0;
				}

				bufferRequest.status = getRequestStatus(bufferRequest);

				if ( bufferRequest.status == FULL_REQUEST ) {
					return 1;
				}

				//GENERATE FILE FOR BODY
				bufferRequest.request._body_file_name = getTempFile();
				std::cerr << "VOY A CREAR EL ARCHIVO " << bufferRequest.request._body_file_name << " PARA LA REQUEST DE ARRIBA" << std::endl;
				bufferRequest.request._body_file_fd = open(bufferRequest.request._body_file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
				if (bufferRequest.request._body_file_fd == -1) {
					std::cerr << "UNABLE TO OPEN FILE " << bufferRequest.request._body_file_name << std::endl;
					throw std::runtime_error("Open error");
				}

				if ( bufferRequest.status == FILLING_BODY ) {
					return readContentLengthBody(bufferRequest, bufferStr);
				} else {
					return readChunkedBody(bufferRequest, bufferStr);
				}
			case FILLING_BODY:
				return readContentLengthBody(bufferRequest, bufferStr);
			case CHUNKED_BODY:
				return readChunkedBody(bufferRequest, bufferStr);
			default:
				// THIS SHOULD NEVER HAPPEN 
				return -1;
		}
	}
	return static_cast<int>(bytes_read);
}

int HTTPServer::handleEvent( int socket, CGIManager & cgiManager ) {
    BufferRequest & bufferRequest = _bufferedRequests[socket];
    switch ( handleRead(socket, bufferRequest) ) {
	    case -1 :
	        return -1 ;
	    case 0 :
            std::cout << "Cerramos el socket: " << socket << std::endl;
            close(socket);
			_bufferedRequests.erase(socket);
	        return 0 ;
	    default :
			HTTPRequest & httpRequest = bufferRequest.request;
			std::cout << "Uri: " << httpRequest.getURI() << std::endl;
			LocationRules & locationRules = *(httpRequest.getLocationRules());
			Response httpResponse;
			std::string file_path = httpRequest.getFilePath();
			int cgi_fd;
			switch (Routing::typeOfResource(file_path, locationRules, httpRequest.getMethod())) {
				case ISCGI:
					std::cerr << "SÍ QUE SOY UN CGI" << std::endl;
					if (httpRequest._body_file_name.empty()) {
						httpRequest._body_file_name = getTempFile();
						std::cerr << "VOY A CREAR EL ARCHIVO " << httpRequest._body_file_name << " PORQUE MI CGI NO TIENE BODY" << std::endl;
						httpRequest._body_file_fd = open(httpRequest._body_file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
						if (httpRequest._body_file_fd == -1) {
							std::cerr << "Webserv Error: unable to open file \"" << httpRequest._body_file_name << "\""  << std::endl;
							throw std::runtime_error("Open error");
						}
						close(httpRequest._body_file_fd);
					}
					cgi_fd = cgiManager.executeCGI(locationRules.getCgiPass(), file_path, httpRequest, socket);
					if (cgi_fd == -1) {
						httpResponse.response_code = 500;
						break;
					}
					_bufferedRequests.erase(socket);
					return cgi_fd;
				case ISDIR:
					httpResponse = Routing::processDirPath(file_path, locationRules);//process directory
					break;
				case ISFILE:
					httpResponse = Routing::processFilePath(file_path);//process file        
					break;
				case ISNAM:
					httpResponse.response_code = 405;
					break;
				default:
					httpResponse.response_code = 404;
			}
		if (!httpRequest._body_file_name.empty()) {
			cgiManager.eraseFile(httpRequest._body_file_name);
		}
		if ( httpResponse.response_code != 200 && httpResponse.response_code != 302 ) {
			Routing::errorResponse(httpResponse, locationRules);
		}
		sendResponse(socket, httpResponse);
		std::cout << "Cerramos el socket: " << socket << std::endl;
    	close(socket);
		_bufferedRequests.erase(socket);
		return 0 ;
	}
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

std::map<std::string, std::string> getContentMap() {
    std::map<std::string, std::string> contentMap;
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
    return contentMap;
}

std::string getContentType(std::string file_path) {
    static std::map<std::string, std::string> ContentMap;

    if (ContentMap.empty()) {
        ContentMap = getContentMap();
    }
    std::vector<std::string> path = split_char(file_path, '/');
    std::string file = path.empty()? "" : path.back();
    std::vector<std::string> file_split = split_char(file, '.');
    std::string extension;
    std::string content_type;
    if (file_split.size() > 1) {
        extension = file_split.back();
        content_type = ContentMap[extension];
        if (content_type == "")
            content_type = "text/plain";
    }
    else {
        content_type = "text/plain";
    }
    return (content_type);
}

int HTTPServer::sendResponse(int socket, Response & httpResponse)
{   
    ResponseCode response_codes;
    std::string date = getDate();
    std::stringstream response;

    response << "HTTP/1.1 " << httpResponse.response_code << " " << response_codes.getCodeString(httpResponse.response_code) << "\r\n";
    response << "Content-Type: " << getContentType(httpResponse.file_path) << "\r\n";
    response << "Content-Length: " << httpResponse.string_body.size() << "\r\n";
	for ( std::map<std::string, std::string>::iterator iter = httpResponse.headers.begin(); iter != httpResponse.headers.end(); iter++ ) {
		response << iter->first << ": " << iter->second << "\r\n";
	}
    response << "\r\n";
	response << httpResponse.string_body;
	std::cout << "\n---Response---\n" << response.str() <<  "---" << std::endl;
    send(socket, response.str().c_str(), response.str().size(), 0);
    return -1;
}
