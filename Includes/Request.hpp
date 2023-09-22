/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:10 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/22 17:42:58 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"

class Request
{
    protected:
        std::string     _method;
        std::string     _path;
        std::string     _httpVersion;
        std::string     _responseStatus;
        std::string     _header_before;
        std::string     _body;
        std::string     _contentType;
        std::map<std::string, std::string> _headers;
        const char*     _bodyFile;
    public:
        Request();
        int                 parseHttpRequest(const std::string& requestBuffer, int new_socket);
        std::string         getPath() const;
        std::string         getMethod() const;
        std::string         getHttpVersion() const;
        std::string         getResponseStatus() const;
        std::string         GetFileExtention( void ) const;
        const std::string&  getContentType( void ) const;
        void                setResponseStatus(const std::string& status);
        int                 processChunk(const std::string& buffer); // int socket ?
        int                 parseRequest();
        void                processBody();
        ~Request();
};