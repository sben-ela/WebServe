/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:54:36 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/21 20:45:20 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configuration.hpp"

class Servers
{
    private:
        std::vector<Configuration>   _servers;
    public:
        Servers() {};
        int    ConfigFileParse(std::string file);
        const std::vector<Configuration>  &getServers( void ) const {
            return (_servers);
        }
        void printServerData() const;
        ~Servers() {};
};