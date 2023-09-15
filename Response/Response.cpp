#include "Response.hpp"


std::string     _method = "GET";
std::string     _path = "image.png";
std::string     _contentType = "mage.png";


void Get(int socket)
{
    std::string header = "HTTP/1.1 200 OK\r\n Content-Type: " + _contentType + "\r\n\r\n";
    std::ifstream file(_path.c_str(), std::ios::binary);
    send(socket, header.c_str(), strlen(header.c_str()), 0);

    const int buffer_size = 2312771;
    char buff[buffer_size];

    while (file) {
        file.read(buff, buffer_size);
        int bytes_read = file.gcount();
        
        if (bytes_read > 0) {
            if (send(socket, buff, bytes_read, 0) < 0) {
                std::cout << "send failed" << std::endl;
                exit(141);
            }
        }
    }
    close(socket);
}


// void    response(const Request &request)
// {
//     if (request.getMethod() == "GET")
//     {
//         Get(request);
//     }
//     else if (request.getMethod() == "POST")
//     {

//     }
// }

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#define PORT 8080


int main() {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Listening on port " << PORT << "..." << std::endl;

    // Accept a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        return 1;
    }

    Get(new_socket);
    std::cout << "Last Line" << std::endl;
    sleep(2);
    return 0;
}


