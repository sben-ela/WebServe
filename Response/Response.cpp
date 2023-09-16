#include "Response.hpp"


void ft_putstr(char *str)
{
    int i = 0;
    while (i < BUFFER_SIZE)
    {
        if (write (1 , &str[i], 1) < 0)
        {
            std::cout << "putstr failed : " << str[i] << std::endl;
            exit (0);
        }
        i++;
    }
}



void    Get(const Client &client)
{
    std::string header = client.GetHttpVersion() + " 200 OK\r\nContent-Type: "
    + client.GetConetType() + "\r\n\r\n";
    send(client.GetSocketId(), header.c_str(), header.size(), 0);
    char buff[BUFFER_SIZE];
    int fd = open (client.GetPath().c_str(), O_RDONLY);

    while (read(fd, buff, BUFFER_SIZE) > 0)
    {
        ft_putstr(buff);
        if (write (client.GetSocketId() , buff, BUFFER_SIZE) < 0)
        {
            std::cout << "write failed" << std::endl;
            exit (141);
        }
    }
    close (fd);
}

void    Post(const Client& client)
{
    
}

void    Response(const Client &client)
{
    if (client.GetMethod() == "GET")
        Get(client);
    else if (client.GetMethod() == "POST")
        Post();
    // else if (client.GetMethod() == "DELETE")
    //     Delete();
}

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>


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

    // std::cout << "Listening on port " << PORT << "..." << std::endl;

    // Accept a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        return 1;
    }

    Response(Client("GET" , "gif.gif", "image/gif", "HTTP/1.1 ", new_socket));

    return 0;
}

