/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:17:48 by sben-ela          #+#    #+#             */
/*   Updated: 2023/09/12 11:09:39 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

#define listenIP    1 
#define listenPORT  2 
#define IPPORT      3
#define PORTIP      4

class Listen
{
    private :
        std::string _ipAddress;
        int         _port;
        int         _status;
    public :
        Listen(){}
        Listen(std::string &listen);
        bool   IsPort(const std::string &str) const;
        bool   IsIp(const std::string &str) const;
        int getStatus(){
            return(_status);
        }
        void info(){
            std::cout << "_ipAddress : " << _ipAddress << std::endl;
            std::cout << "_port : " << _port << std::endl;
            std::cout << "_status : " << _status << std::endl;

        }
};

class location
{
    private :
        std::vector<std::string> _allowMethods;
        std::vector<std::string> _index;
        std::string _autoindex;
        std::string _location;
    public :
        // location(std::vector<std::string> allowMethods , std::vector<std::string>  index, std::string autoindex, std::string  location);
        // void Setallow_methods();
        // void Setindex();
        // void Setautoindex();
        // void Setlocation();
};


class Config
{
    private :
        std::string listen;
        std::vector<std::map<int, std::string> > error_page;
        std::vector<location> Location;

    public :
};



// ? listen IP : If you specify only an IP address without a port, Nginx will listen on that
// ? IP address for incoming HTTP requests on the default HTTP port which is port 80

// ? listen PORT : If you specify only a port without an IP address, Nginx will listen on all available
// ? network interfaces (all IP addresses) for incoming HTTP requests on the specified port.

// ? listen IP PORT : Nginx will listen on the specified IP address and port for incoming HTTP requests.

// ? listen PORT IP : When you specify the IP address after the port, Nginx will behave
// ? the same way as when you specify listen IP PORT