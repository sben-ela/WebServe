/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:21 by aybiouss          #+#    #+#             */
/*   Updated: 2023/10/06 15:43:43 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

Client::Client() : _status(0) {}

void    Client::set_server(Configuration p)
{
    _client_server = p;
}
void    Client::set_socket(int socket)
{
    _socketId = socket;
}

const int& Client::GetSocketId( void ) const
{       
    return (_socketId);
}

Client::Client(const Client& other)
{
    if (this != &other)
    {
        this->_socketId = other._socketId;
        _readStatus = other._readStatus;
        this->_client_server = other._client_server;
        _status = other._status;
        this->response = other.response;
    }
}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        this->_socketId = other._socketId;
        _readStatus = other._readStatus;
        _status = other._status;
        this->_client_server = other._client_server;
        this->response = other.response;
    }
    return *this;
}

const Configuration&      Client::getServer( void ) const
{
    return (_client_server);
}

void    Client::fullMapEnv()
{
    _mapEnv["CONTENT_TYPE"] = get_content_type();
    _mapEnv["QUERY_STRING"] = response.getQueryString();
    _mapEnv["PATH_INFO"] = response.getPath(); 
}

void    Client::fullEnv()
{
    fullMapEnv();
    std::cout << _mapEnv.size() << std::endl;
    _env = new char*[_mapEnv.size() + 1];
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = _mapEnv.begin(); it != _mapEnv.end();)
    {
        _env[i] = strdup((char *)(it->first + "=" + it->second).c_str());
        it++;
        i++;
    }
    _env[i] = NULL;
}

void    Client::deleteEnv()
{
    for (size_t i = 0; _env[i]; i++)
        free(_env[i]);
    delete [] _env;
}

const char* Client::get_content_type( void )
{
    const char *last_dot = strrchr(response.getPath().c_str(), '.');
    if (last_dot)
    {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".mp4") == 0) return "video/mp4";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
        if (strcmp(last_dot, ".py") == 0) return "text/plain";
        if (strcmp(last_dot, ".php") == 0) return "text/plain";
    }
    return ("text/plain");
}

Client::~Client() {}