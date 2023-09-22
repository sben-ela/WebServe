/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:14 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/21 19:27:16 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"

class Socket
{
    public:
        int function();
        void setnonblocking(int *sock);
};