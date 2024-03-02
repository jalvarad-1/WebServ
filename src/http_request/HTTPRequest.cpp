#include "HTTPRequest.hpp"
#include <sstream>
#include <iostream>
#include <stdlib.h>

void trim(std::string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");

    if (start == std::string::npos)
        s = "";
    else
        s = s.substr(start, end - start + 1);
}

HTTPRequest::HTTPRequest(void) {//Dummy constructor
	_body_file_fd = -1;
}

HTTPRequest::HTTPRequest(const std::string& raw_request, ServerConfig& serverConfig)
{
    if (!parse(raw_request)) {
        _error_code = 400;
		return ;
	}
	_body_file_fd = -1;
    _location_rules = &Routing::determineResourceLocation(serverConfig, *this);
    if (!Routing::isAllowedMethod(_method, _location_rules->getAllowedMethods())) {
		_error_code = 405;
		return ;
	}
	if (!_location_rules->getRedirect().empty()) {
        _error_code = 302;
        return ;
	}
    int content_lenght = returnContentLength();
    if (content_lenght != -1 && _location_rules->getMaxBodySize() != 0 && content_lenght > _location_rules->getMaxBodySize()) {
        _error_code = 413;
        return ;
    }
	_file_path = Routing::createFilePath(*_location_rules, *this);
	if (_method == "DELETE") {
		_headers["Content-Disposition"] = "filename=\"" + _file_path + "\"";
		_file_path = _location_rules->getDeleteCGI(); //PATH DEL CGI DE DELETE
		if (_file_path.empty()) {
			_error_code = 500;
			return ;
		}
		// WHATEVER DELETE CGI NEEDS
	}
    _error_code = 200;
}

bool HTTPRequest::methodAcceptsBody() const
{
        std::string methods = METHODS_WITH_BODY;
        return methods.find(_method) != std::string::npos;
}

 int HTTPRequest::returnContentLength() const
{
    std::string contentLengthStr = getHeader("Content-Length");
	if ( contentLengthStr.empty() )
		return -1 ;
	return std::atoi(contentLengthStr.c_str());
}

bool HTTPRequest::parse(const std::string& raw_request) {
    std::istringstream ss(raw_request);
    std::string line;

    std::getline(ss, line);
    std::istringstream request_line(line);
    request_line >> _method >> _uri >> _http_version;
	if ( _method.empty() || _uri.empty() || _http_version.compare("HTTP/1.1") ) {
		return false ;
	}
    _http_version = "HTTP/1.1";
    while (std::getline(ss, line) && !line.empty() && line != "\r")
    {
        std::size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
        {
            return false;
        }

        std::string header_name = line.substr(0, colon_pos);
        std::string header_value = line.substr(colon_pos + 1);

        trim(header_name);
        trim(header_value);

        if (!header_name.empty() && !header_value.empty())
            _headers[header_name] = header_value;
        else
        {
            return false;
        }
    }
    return true;// devuelve true si todo salió bien, o false si hubo algún error
}

std::string HTTPRequest::getMethod() const
{
    return _method;
}

std::string HTTPRequest::getURI() const {
    return _uri;
}

std::string HTTPRequest::getVersion() const {
    return _http_version;
}

std::string HTTPRequest::getHeader(const std::string& header_name) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(header_name);
    if (it != _headers.end()) {
        return it->second;
    }
    return "";
}

std::string HTTPRequest::getBody() const
{
    return _body;
}

int HTTPRequest::getErrorCode() const
{
        return _error_code;
}

std::map<std::string, std::string> HTTPRequest::getHeaders() const {
    return _headers;
}

std::string HTTPRequest::getPathInfo() const {
    return _path_info;
}

std::string HTTPRequest::getFilePath() const {
    return _file_path;
}


LocationRules * HTTPRequest::getLocationRules() const {
	return _location_rules;
}

void HTTPRequest::setPathInfo(std::string path_info) {
    if (path_info.empty())
        path_info += " ";
    _path_info = path_info;
}
