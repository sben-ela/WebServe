/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:10 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/08 10:10:43 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"

class Request
{
    private:
        std::string     _method;
        std::string     _path;
        std::string     _httpVersion;
        std::string     _responseStatus;
        std::map<std::string, std::string> _header;
        const char*     _bodyFile;
    public:
        Request();
        void parseHttpRequest(const std::string& requestBuffer, int new_socket);
        std::string getPath() const;
        std::string getMethod() const;
        std::string getHttpVersion() const;
        std::string getResponseStatus() const;
        void setResponseStatus(const std::string& status);
        ~Request();
};