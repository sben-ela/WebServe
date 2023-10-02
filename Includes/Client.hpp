/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:08 by aybiouss          #+#    #+#             */
/*   Updated: 2023/10/01 19:49:12 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "Configuration.hpp"
#include "Response.hpp"

struct   Methods
{
    bool _get;
    bool _post;
    bool _delete;
};


class Client
{
    private:
        int         _socketId;
        Configuration   _client_server;
    public :
        int         _content_fd;
        ssize_t     _readStatus;
        int         _status;
        Methods     methods;
        Response    response;
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        const int&          GetSocketId( void ) const;
        const Configuration&      getServer() const;
        void    set_server(Configuration p);
        void    set_socket(int socket);
        ~Client();
};