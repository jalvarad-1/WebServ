/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarcu <cmarcu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 17:42:44 by cmarcu            #+#    #+#             */
/*   Updated: 2023/12/13 14:58:27 by cmarcu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include <unistd.h>

CGI::CGI(const std::string& script_path, const std::map<std::string, std::string>& request_headers) : _script_path(script_path), _req_headers(request_headers)
{
	//TODO decidir qué atributos tiene CGI
}

CGI::~CGI()
{
}

char **CGI::convert_env() {
	char **env = new char*[this->_env.size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = this->_env.begin(); it != this->_env.end(); ++it) {
		std::string env_line = it->first + "=" + it->second;
		env[i] = new char[env_line.size() + 1];
		env[i] = strcpy(env[i], env_line.c_str());
		i++;
	}
	env[i] = NULL;
	return env;
}

std::string CGI::execute(HTTPRequest& request) {
	std::string output;
	int pipefd[2];
	pipe(pipefd);

	pid_t pid = fork();
	if (pid == -1)
	{
		//TODO lanzar error
	}
	else if (pid == 0) // child process
	{
		close(pipefd[0]); // Close unused read end
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
		build_env(request);
		char **envp = convert_env();
		execve(this->_script_path.c_str(), NULL, envp);
		//TODO lanzar error: recordar que si pasa de execve es que la ejecución ha fallado. Si todo va bien, execve no retorna
	}
	else // parent process
	{
		close(pipefd[1]); // Close unused write end
		char buff[1024]; //TODO hacer macro con tamaño de buffer
		while (read(pipefd[0], buff, sizeof(buff)) > 0)
			output += buff; //TODO ver si es mejor un ostream en vez de +=
		close(pipefd[0]);
		waitpid(-1, NULL, 0);
	}
	//TODO delete envp and all allocated memory
	return output;
}

void CGI::build_env(HTTPRequest& request) {

	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["SCRIPT_NAME"] = this->_script_path;
	this->_env["QUERY_STRING"] = request.getQuery(); //TODO define getQuery inside request class
	this->_env["CONTENT_TYPE"] = request.getHeaders()["Content-Type"];
	this->_env["CONTENT_LENGTH"] = request.getBody().length(); //Tengo mis dudas de que el content length sea este o haya ue actualizarlo también una vez ejecutao el script co lo que devuelva
	this->_env["REQUEST_URI"] = request.getURI();
	this->_env["REMOTE_ADDR"] = //TODO get host port de la config del server
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = request.getHeaders()["Hostname"]; //TODO si no hay hostname poner remoteaddr
	this->_env["SERVER_PORT"] = request.getHeaders()["Port"]; //TODO no sé si ese header existe hehe
	//TODO añadir cualquier otra necesaria. Cuáles?
}
