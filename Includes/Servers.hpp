/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:54:36 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/22 14:39:29 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configuration.hpp"
#include "Socket.hpp"
#include "Client.hpp"

class Servers
{
    private:
        std::vector<Configuration>   _servers;
        std::vector<Client>          _client;
    public:
        Servers() {};
        int    ConfigFileParse(std::string file);
        void printServerData() const;
        int AllServers();
        void checkServers();
        ~Servers() {};
};