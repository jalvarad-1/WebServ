/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarcu <cmarcu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 16:55:24 by cmarcu            #+#    #+#             */
/*   Updated: 2023/12/09 18:02:44 by cmarcu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/HTTPServer.hpp"

class HTTPRequest;

class CGI {
	private:
		std::map<std::string, std::string> _req_headers;
		std::string _script_path;

	public:
		CGI(const std::string& scriptPath, const std::map<std::string, std::string>& requestHeaders);
		~CGI();

		void execute(HTTPRequest& request);
		void build_env(HTTPRequest& request);

};
