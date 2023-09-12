/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:03 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/12 21:47:13 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define PORT 8080 // Where the clients can reach at
#define MAX_CLIENTS 10 // Maximum number of clients to handle

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include "./Includes/Socket.hpp"
#include "./Includes/Request.hpp"
#include "./Includes/CommonEntity.hpp"
#include "./Includes/ConfigFile.hpp"
#include "./Includes/Configuration.hpp"
#include "./Includes/Location.hpp"
#include "./Includes/Request.hpp"
#include "./Includes/Response.hpp"
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>      // For input and output
#include <cstring>       // For string manipulation and error handling
#include <cstdlib>       // For standard library functions
#include <cerrno>        // For error numbers and error handling
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For IP address structures and constants
#include <unistd.h>      // For read, write, and close functions
#include <fcntl.h>       // For file control options and non-blocking sockets
#include <sys/select.h>  // For the select function
#include <algorithm>
#include <string>        // For string manipulation
#include <exception>
#include <climits>
#include <sys/types.h>
#include <sys/wait.h>