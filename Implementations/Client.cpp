/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:21 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/29 19:02:27 by sben-ela         ###   ########.fr       */
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

Client::~Client() {}