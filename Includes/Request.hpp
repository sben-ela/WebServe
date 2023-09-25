/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:10 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/25 16:16:33 by sben-ela         ###   ########.fr       */
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
        const char*     _bodyFile;
        std::map<std::string, std::string>  _headers;
        std::map<int, std::string>          _statusCode;
    public:
        Request();
        std::map<std::string, std::string> getHeaders(){
            return (_headers);
        }
        int                 parseHttpRequest(const std::string& requestBuffer, int new_socket);
        std::map<int, std::string> getStatusCode( void ) const;
        std::string         getPath() const;
        std::string         getMethod() const;
        std::string         getHttpVersion() const;
        std::string         getResponseStatus() const;
        std::string         GetFileExtention( void ) const;
        std::string         getContentType( void ) const;
        void                setPAth(std::string newPath);
        void                setResponseStatus(const std::string& status);
        int                 processChunk(const std::string& buffer); // int socket ?
        int                 parseRequest();
        void                processBody();
        void                CreateStatusCode( void ); // ! new
        ~Request();
};