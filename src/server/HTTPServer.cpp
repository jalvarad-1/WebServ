#include "HTTPServer.hpp"
#include "MultiServer.hpp"
#include "../utils/utils.hpp"
#include "response_code/ResponseCode.hpp"
#include <ctime>
#include "routing/Routing_ns.hpp"

BufferRequest::BufferRequest( void ) {
	content_length = -1;
	status = NEW_REQUEST;
}

HTTPServer::HTTPServer(int domain, int service, int protocol,
            int port, u_long interface, int bklg, const ServerConfig & serverConfig):
            _serverConfig(serverConfig)
{
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

bool HTTPServer::parseChunk(std::string & bufferStr, std::string & body) {
	size_t firstRN = bufferStr.find("\r\n");
	if (firstRN == std::string::npos)
		return false ;
	char *pEnd;
	double characterN = std::strtod(bufferStr.substr(0, firstRN).c_str(), &pEnd);
	if (*pEnd != '\0') {
		//TODO throw exception
	}
	if (characterN == 0)
		return true ;
	firstRN += 2;
	size_t secondRN = bufferStr.find("\r\n", firstRN);
	if (secondRN == std::string::npos)
		return false ;
	if (static_cast<double>(secondRN) != characterN) {
		//TODO throw exception
	}
	body.append(bufferStr.substr(firstRN, secondRN));
	secondRN += 2;
	bufferStr.erase(bufferStr.begin() + firstRN + secondRN);
	return parseChunk(bufferStr, body) ;
}

int HTTPServer::handleRead( int socket, BufferRequest & bufferRequest ) {
	std::string & bufferStr = bufferRequest.buffer_str;
	ssize_t bytes_read = readFromFd(socket, bufferStr);
	if (bytes_read > 0) {
		size_t rnrn;
		switch (bufferRequest.status) {
			case NEW_REQUEST:
				// BUSCAMOS RNRN Y GENERAMOS LA NUEVA REQUEST
				rnrn = bufferStr.find("\r\n\r\n");
				if ( rnrn == std::string::npos ) {
					std::cerr << "NO HE ENCONTRADO EL FINAL" << std::endl;
					return -1 ;
				}
				rnrn += 4;
				bufferRequest.request = HTTPRequest(bufferStr.substr(0, rnrn));
				std::cout << "\n---Request---\n" << bufferStr.substr(0, rnrn) << "---" << std::endl;
				if (bufferRequest.request.getHeader("Transfer-Encoding") == "chunked") {
					bufferRequest.status = CHUNKED_BODY;
					bufferStr.erase(bufferStr.begin() + rnrn);
					return -1 ;
				}
				bufferRequest.content_length = bufferRequest.request.returnContentLength();
				if ( bufferRequest.content_length < 0 )	
					return 1 ;
				bufferRequest.status = FILLING_BODY;
				bufferStr.erase(bufferStr.begin() + rnrn);
				return -1 ;
			case FILLING_BODY:
				// COMPROBAMOS EL CONTENT_LENGTH Y ACTUAMOS SEGÚN
				if ( static_cast<int>(bufferStr.size()) < bufferRequest.content_length ) {
					return -1 ;
				}
				bufferRequest.request._body = bufferStr;
				std::cout << "\n---Body---\n" << bufferStr << "---" << std::endl;
				return 1 ;
			case CHUNKED_BODY:
				// BUSCAMOS DOS RN Y APPENDEAMOS AL BODY DESPUÉS DE PASAR POR UNA FUNCIÓN DE PARSEO
				if (parseChunk(bufferStr, bufferRequest.request._body))  {
					return 1 ;
				}
				return -1 ;
			default:
				// NUNCA DEBERÍA PASAR
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
			HTTPRequest httpRequest = bufferRequest.request;
			std::cout << "Uri: " << httpRequest.getURI() << std::endl;
			LocationRules locationRules = Routing::determineResourceLocation(_serverConfig, httpRequest);
			if (!Routing::isAllowedMethod(httpRequest.getMethod(), locationRules.getAllowedMethods())) {
				// TODO: devolver response 405
				// response.response_code = 405;
				// errorResponse(response, locationRule);
				return 0 ;
			}
			if (!locationRules.getRedirect().empty()) {
				// TODO: devolver response 302
				// response.response_code = 302;
				// response.headers["Location"] = locationRule.getRedirect();
				return 0 ;
			}
			std::string file_path = Routing::createFilePath(locationRules, httpRequest);
			Response httpResponse;
			switch (Routing::typeOfResource(file_path, locationRules)) {
				case ISCGI:
					std::cerr << "SÍ QUE SOY UN CGI" << std::endl;
					return cgiManager.executeCGI(locationRules.getCgiPass(), file_path, httpRequest, socket);
				case ISDIR:
					httpResponse = Routing::processDirPath(file_path, locationRules);//process directory
					break;
				case ISFILE:
					httpResponse = Routing::processFilePath(file_path);//process file        
					break;
				default:
					httpResponse.response_code = 404; // TODO send response 404
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
    std::cout << "segunda linea file path" << std::endl;
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
    std::cout << "getContentType: " << content_type << std::endl;
    return (content_type);
}

int HTTPServer::sendResponse(int socket, Response & httpResponse)
{   
    ResponseCode response_codes;
    std::string date = getDate();
    std::stringstream response;

    response << "HTTP/1.1 " << httpResponse.response_code << " " << response_codes.get_code_string(httpResponse.response_code) << "\r\n";
    response << "Content-Type: " << getContentType(httpResponse.file_path) << "\r\n";
    response << "Content-Length: " << httpResponse.string_body.size() << "\r\n";
	for ( std::map<std::string, std::string>::iterator iter = httpResponse.headers.begin(); iter != httpResponse.headers.end(); iter++ ) {
		response << iter->first << ": " << iter->second << "\r\n";
	}
    response << "\r\n";
	response << httpResponse.string_body;
	std::cout << "\n---Response---\n" << response.str() <<  "---" << std::endl;
    send(socket, response.str().c_str(), response.str().size(), 0);
    std::cout << "Cerramos el socket: " << socket << std::endl;
    return -1;
}
