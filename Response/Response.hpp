/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:59:09 by sben-ela          #+#    #+#             */
/*   Updated: 2023/09/21 20:39:00 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "../Includes/Configuration.hpp"
#define BUFFER_SIZE 1024

void    ft_Response(const Client &client, const Servers &servers);


