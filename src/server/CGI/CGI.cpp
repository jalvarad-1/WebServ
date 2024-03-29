#include "CGI.hpp"
#include <iostream>
#include <fstream>

#include <cstdlib>

#define wr 1
#define rd 0

int openFile(std::string file) {
    int fileDescriptor = open(file.c_str(), O_RDONLY);
    if (fileDescriptor == -1) {
        std::cerr << "Error: Could not open file " << file << std::endl;
        return -1;
    }
    return fileDescriptor;
}

int CGI::set_error(int code, std::string body) {
    ret.response_code = code;
    ret.string_body = body;
    return (-1);
}

void CGI::parse_output(std::string output) {
    std::size_t pos = output.find("\n\r", 0);
    if (pos != std::string::npos) {
        //Guardamos el body desde justo después del salto de línea
        ret.string_body = output.substr(pos + 3);
        std::string headers;
        headers = output.substr(0, pos + 1);
        std::size_t pos = 0;
        std::size_t prevPos = 0;
        std::string linea;
        while ((pos = headers.find("\n", prevPos)) != std::string::npos) {
            linea = headers.substr(prevPos, pos - prevPos);
            ret.headers[linea.substr(0, linea.find(":"))] = linea.substr(linea.find(":") + 2);
            prevPos = pos + 1;
        }
    }
    else 
        ret.string_body = output;
}

int CGI::child_process(int (&pipefd)[2], int rd_fd) {
    if (dup2(rd_fd, STDIN_FILENO) == -1 || close(pipefd[rd]) == -1 ||
        dup2(pipefd[wr], STDOUT_FILENO) == -1 || close(pipefd[wr]) == -1 ) {
		write(pipefd[wr], "Status: 500 Internal Server Error\r\n\r\n", 37);
        exit(500);
        return -1;
    }

    std::stringstream salida;

    salida << "Soy el argv: " << _argv[1] << "||" << std::endl; 
	std::cerr << salida.str().c_str();
    execve(_cgi_path, _argv, _envp);
	write(1, "Status: 500 Internal Server Error\r\n\r\n", 37);
    exit(500);
    return (-1);
}

int CGI::exec_cgi(std::string body_filename, pid_t *ret_pid) {
    std::cerr << "Soy el argv del padre (funciona porfi): " << _argv[1] << std::endl;
	int rd_fd = open(body_filename.c_str(), O_RDONLY);
	if (rd_fd == -1) {
		std::cerr << "ERROR WHEN TRYING TO OPEN " << body_filename << std::endl;
		perror("open");
		return -1;
	}
    int pipefd[2];
    if (pipe(pipefd) == -1) {
		perror("pipe");
        return -1;
    }
    pid_t pid = fork();
    if (pid == -1) {
		perror("fork");
        return -1;
    }
    else if (pid == 0) {
        if (this->child_process(pipefd, rd_fd) == -1) {
            return -1;
        }
    }
    else {
        *ret_pid = pid;
        close(pipefd[wr]);
        return pipefd[rd];
    }
    return -1;
}

void CGI::set_env(std::map<std::string, std::string> map) {
    _envp = new char*[map.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++) {
        std::string tempVec = it->first + "=" + it->second;
        _envp[i] = new char[tempVec.size() + 1];
        strcpy(_envp[i], tempVec.c_str());
        _envp[i][tempVec.size()] = '\0';
        i++;
    }
    _envp[i] = NULL;
}

void CGI::set_args(std::vector<std::string> vec) {
    _argv = new char*[vec.size() + 1];
    int i = 0;
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++) {
        _argv[i] = new char[it->size() + 1];
        strcpy(_argv[i], it->c_str());
        _argv[i][it->size()] = '\0';
        i++;
    }
    _argv[i] = NULL;
}

CGI::CGI(std::string cgi_path) {
    _cgi_path = new char[cgi_path.size() + 1];
    strcpy(_cgi_path, cgi_path.c_str());
    _cgi_path[cgi_path.size()] = '\0';
    // _file_path = file_path;
}

CGI::~CGI() {
    int i = 0;
    while (_envp[i] != NULL) {
        delete[] _envp[i];
        i++;
    }
    delete[] _envp;

    i = 0;
    while (_argv[i] != NULL) {
        delete[] _argv[i];
        i++;
    }
    delete[] _argv;
    delete[] _cgi_path;
}
