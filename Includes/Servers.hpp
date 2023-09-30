/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:54:36 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/29 18:32:06 by sben-ela         ###   ########.fr       */
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
        Servers();
        int    ConfigFileParse(std::string file);
        void printServerData() const;
        int AllServers();
        void checkServers();
        ~Servers() {};
};