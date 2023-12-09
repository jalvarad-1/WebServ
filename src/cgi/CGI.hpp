/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarcu <cmarcu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 16:55:24 by cmarcu            #+#    #+#             */
/*   Updated: 2023/12/09 21:09:02 by cmarcu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/HTTPServer.hpp"

class HTTPRequest;

class CGI {
	private:
		std::map<std::string, std::string>& _env;
		std::string _script_path;

	public:
		CGI(const std::string& scriptPath, const std::map<std::string, std::string>& requestHeaders);
		~CGI();

		std::string execute(HTTPRequest& request);
		void build_env(HTTPRequest& request);
		char **convert_env();
};
