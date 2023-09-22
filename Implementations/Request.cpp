/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/22 23:20:04 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Request.hpp"

Request::Request() {}

// Function to parse an HTTP request
int Request::parseHttpRequest(const std::string& requestBuffer, int new_socket) {
    (void)new_socket;
    // std::cout << requestBuffer.find("Transfer-Encoding: chunked") << std::endl;
    // std::cout << requestBuffer << std::endl;
    int i = processChunk(requestBuffer);
    if (!i)
        return parseRequest();
    return 1;
}

int		Request::processChunk(const std::string &buffer)
{
    std::string	chunks;
    // nrd requests vector and check in function of the socket (fd)
    if (_header_before.find("\r\n\r\n") == std::string::npos)
    {
	    _header_before += buffer.substr(0, buffer.find("\r\n\r\n"));
        if (buffer.find("\r\n\r\n") != std::string::npos)
            chunks = buffer.substr(buffer.find("\r\n\r\n") + 4, buffer.length() - 1);
    }
    else
	    chunks = buffer.substr(0, buffer.length() - 1);
    _body += chunks;
    // std::cout << _body << " WOW " << _body.length() << std::endl;
    if (!_body.empty())
    {
        size_t pos = _header_before.find("Content-length: ");
        if (pos != std::string::npos)
        {
            pos += strlen("Content-length: ");
            size_t end_pos = _header_before.find("\r\n", pos);
            std::cout << pos << "   " << end_pos << std::endl;
            if (end_pos != std::string::npos)
            {
                std::string length = _header_before.substr(pos, end_pos - pos);
                size_t len = strtol(length.c_str(), NULL, 10);
                std::cout << "Content-Length as integer: " << len << std::endl;
                processBody();
                if (len == _body.length())
                {
                    _header_before += _body;
                    std::cout << _header_before << std::endl;
                    return 0;
                }
                else
                    return 1;
            }
        }
    }
    else
        return 0;
    return 1;
}

void    Request::processBody()
{
    std::string	subchunk = _body.substr(0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;
	while (chunksize)
	{
		i = _body.find("\r\n", i) + 2;
		body += _body.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = _body.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
    _body.clear();
    _body = body;

    // std::string body = _body;
    // _body.clear();
    // std::istringstream ss(body);
    // std::string line;
    // while (std::getline(ss, line) && !line.empty())
    // {
    //     int length = strtol(line.c_str(), NULL, 16);
    //     if (length)
    //     {
    //         if (std::getline(ss, line) && !line.empty())
    //         {
                
    //         }
    //     }
    // }
}

int Request::parseRequest()
{
    std::istringstream requestStream(_header_before);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus("400 Bad Request");
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus("400 Bad Request");
        return 0;
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return 0;
    }
    std::string forBody;
    bool isContentLengthFound = false;
    std::size_t contentLength = 0;
    // Read and parse headers
    while (std::getline(requestStream, line) && !line.empty()) {
        forBody += line + "\n";
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            _headers[headerName] = headerValue;
            if (_method == "POST" && headerName == "Content-Length")
            {
                try {
                    char* endptr;
                    const char* headerValueCStr = headerValue.c_str();
                    unsigned long parsedContentLength = strtoul(headerValueCStr, &endptr, 10);

                    if (parsedContentLength == ULONG_MAX) { /*endptr == headerValueCStr || *endptr != '\0' || */
                        // Handle invalid Content-Length value
                        setResponseStatus("400 Bad Request");
                        return 0;
                    }
                    contentLength = parsedContentLength;
                    isContentLengthFound = true;
                } catch (const std::exception& e) {
                    // Handle invalid Content-Length value
                    setResponseStatus("400 Bad Request");
                    return 0;
                }
            }
        }
    }
    // std::cout << "*******************" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }
    // std::cout << std::endl;
    // std::cout << "*******************" << std::endl;
    // (void)new_socket;
    if (_method == "POST") {
        if (!isContentLengthFound) {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return 0;
        }
        std::size_t bodyPos = forBody.find("\r\n\r\n");
        if (bodyPos != std::string::npos) {
            // Extract the body
            std::string requestBody = forBody.substr(bodyPos + 4);
            // Write the body to a file
            _bodyFile = "BodyOfRequest.txt";
            std::ofstream outfile(_bodyFile, std::ofstream::binary);
            outfile.write(requestBody.c_str(), contentLength);
            outfile.close();
        }
        else {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return 0;
        }
    }
    setResponseStatus("200 OK");
    return (0);
}

std::string Request::getPath() const
{
    return this->_path;
}

std::string Request::getMethod() const
{
    return this->_method;
}

std::string Request::getHttpVersion() const
{
    return this->_httpVersion;
}

const std::string& Request::getContentType( void ) const
{
    
    return (_contentType);
}

std::string  Request::GetFileExtention( void ) const
{
    std::string extention;
    size_t index;

    index = _path.find('.');
    if (index == std::string::npos)
        return(extention);
    extention = _path.substr(index);
    return (extention);
}

std::string Request::getResponseStatus() const
{
    return this->_responseStatus;
}

void Request::setResponseStatus(const std::string& status) {
    _responseStatus = status;
}

Request::~Request() {}


    // if ()
	// std::string	subchunk = chunks.substr(0, 100);
	// std::string	body = "";
	// int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	// size_t		i = 0;
	// while (chunksize)
	// {
	// 	i = chunks.find("\r\n", i) + 2;
	// 	body += chunks.substr(i, chunksize);
	// 	i += chunksize + 2;
	// 	subchunk = chunks.substr(i, 100);
	// 	chunksize = strtol(subchunk.c_str(), NULL, 16);
	// }
	// std::string requestBuffer = head + "\r\n\r\n" + body + "\r\n\r\n"; // NJM3Ha kamla onsiftha l constructor again and clear the old one
    // std::cout << requestBuffer << std::endl;
    // ! if content length == body.size means the request is done !
    // return 0;