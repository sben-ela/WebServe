/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:10 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/30 16:25:27 by sben-ela         ###   ########.fr       */
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
        std::string    _header_before;
        std::vector<char>     _body;
        std::map<std::string, std::string> _headers;
        std::map<int, std::string>          _statusCode;
        std::string     _contentType;
        const char*     _bodyFile;
        std::string     _ofile;
        int             _chunksize;
        std::string     _bodies;
        std::string     _all;
        bool        _headers_done;
        int         _fd;
    public:
        Request();
        Request(const Request& other);
        Request& operator=(const Request& other);
        int             parseHttpRequest(std::string requestBuffer, int new_socket, size_t bytesread);
        const std::string&     getPath() const;
        const std::string&     getMethod() const;
        const std::string&     getHttpVersion() const;
        const std::string&     getResponseStatus() const;
        std::string         GetFileExtention( void ) const;
        std::string         getContentType( void ) const;
        void                setPath(std::string newPath);
        void            setResponseStatus(const std::string& status);
        int             processChunk(std::string buffer, size_t bytesread); // int socket ?
        int             parseRequest();
        int            processBody();
        size_t          customFind(const std::vector<char>& vec, const std::string& str, size_t start);
        int            parseHeaders();
        std::string     GenerateFile();
        const int&  getFd() const;
        int             countNumbersInString(const std::string& inputString);
        std::string     vectorCharToString(const std::vector<char>& vec);
        std::vector<char> customSubstr(const std::vector<char>& vec, size_t start, size_t length);
        // ! NEW
        std::map<int, std::string> getStatusCode( void ) const;
        void                CreateStatusCode( void );
        // !
        ~Request();
};