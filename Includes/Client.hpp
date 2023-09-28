/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:08 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/27 21:35:37 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "Configuration.hpp"
#include "Response.hpp"



class Client
{
    private : 
        int             _socketId;
        Configuration   _client_server;
    public : 
        Response    response;
        Client() {};
        const int&          GetSocketId( void ) const;
        const Configuration&      getServer( void ) const;
        void    set_server(Configuration p);
        void    set_socket(int socket);
};