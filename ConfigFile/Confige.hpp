# pragma once

#include <vector>
#include <map>
#include <iostream>


class location
{
    std::vector<std::string> allow_methods;
    std::vector<std::string> index;
    std::string autoindex;
    std::string location;
    public :
        void Setallow_methods();
        void Setindex();
        void Setautoindex();
        void Setlocation();
};

class Confige
{
    private :
        std::vector<std::map<std::string, int>> error_page;
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

