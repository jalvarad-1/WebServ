/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarcu <cmarcu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 17:42:44 by cmarcu            #+#    #+#             */
/*   Updated: 2023/12/09 17:48:33 by cmarcu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(const std::string& script_path, const std::map<std::string, std::string>& request_headers) : _script_path(script_path), _req_headers(request_headers)
{
}

CGI::~CGI()
{
}

