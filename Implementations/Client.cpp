/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:21 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/27 21:35:49 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

// Client::Client(std::string method, std::string path, std::string fileExtention, std::string contentType, std::string httpVersion, int socketId)
// {
//     _method = method;
//     _path = path;
//     _fileExtention = fileExtention;
//     _contentType = contentType;
//     _httpVersion = httpVersion;
//     _socketId = socketId;
// }

void    Client::set_server(Configuration p)
{
    _client_server = p;
}
void    Client::set_socket(int socket)
{
    _socketId = socket;
}

const Configuration&      Client::getServer( void ) const
{
    return (_client_server);
}

const int& Client::GetSocketId( void ) const
{       
    return (_socketId);
}

// const std::string& Client::GetMethod( void ) const
// {
//     return (_method);
// }

// const std::string& Client::GetPath( void ) const
// {
//     return (_path);
// }

// const std::string& Client::GetConetType( void ) const
// {
//     return (_contentType);
// }

// const std::string& Client::GetHttpVersion( void ) const
// {
//     return (_httpVersion);
// }

// const std::string&  Client::GetFileExtention( void ) const
// {
//     return (_fileExtention);
// }
