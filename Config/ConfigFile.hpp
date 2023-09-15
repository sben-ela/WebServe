/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:09:26 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 16:55:39 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include <vector>
#include <map>
#include <iostream>

// ? listen IP : If you specify only an IP address without a port, Nginx will listen on that
// ? IP address for incoming HTTP requests on the default HTTP port which is port 80

// ? listen PORT : If you specify only a port without an IP address, Nginx will listen on all available
// ? network interfaces (all IP addresses) for incoming HTTP requests on the specified port.

// ? listen IP PORT : Nginx will listen on the specified IP address and port for incoming HTTP requests.

// ? listen PORT IP : When you specify the IP address after the port, Nginx will behave
// ? the same way as when you specify listen IP PORT