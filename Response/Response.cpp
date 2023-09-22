/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
/*   Updated: 2023/09/23 00:51:54 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Webserv.hpp"
#include "../Includes/Configuration.hpp"
#include "../Includes/Client.hpp"
#define FILESIZE 10
#define EXFIALE 13 


void    SeparatesHeader(std::string& str)
{
    str = str.substr(str.find("\r\n\r\n") + 4);
}


std::string GenerateFile( void )
{
    std::string Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefjhiGklmnopqrstuvwxyz+-_";
    std::string file;
    
    for (size_t i = 0; i < FILESIZE; i++)
    {
        file += Base[rand() % Base.size()];
    }
    return("/tmp/" +  file);
}


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


size_t getLocationIndex(const Client& client)
{
    size_t index;

	index = client.getServer().getLocations().size() - 1;;
	while (index > 0)
	{
		if (client.getServer().getLocations()[index].getpattern() == client.response.getPath().substr(0, client.getServer().getLocations()[index].getpattern().size()))
			return(index);
		index--;
	}
	return(0);
}

std::string getFileName(const std::string& path, size_t first)
{
    std::string fileName = path.substr(first);
    if (fileName[0] != '/')
        return("/" + fileName);
    return(fileName);
}

std::string  getFilePath(const Client& client)
{
    std::string filePath;
	size_t index;

	index = getLocationIndex(client);
        
	filePath = client.getServer().getRoot() + getFileName(client.response.getPath()
	, client.getServer().getLocations()[index].getpattern().size());
	return (filePath);
}

void    Get(const Client &client)
{
    char buff[BUFFER_SIZE];
    std::string header;
    std::stringstream ss;
    struct stat statbuffer;
    int fd;


    // std::cout << getFilePath(client).c_str() << std::endl;;
    if (client.response.GetFileExtention() == ".php" || client.response.GetFileExtention() == ".py")
    {
        std::string outfile = GenerateFile();
        // std::cout << outfile << std::endl;
        int pid  = fork();
        if (!pid)
        {
            std::map<std::string, char*> intrepreter;
            intrepreter[".php"] = (char *)"/usr/bin/php-cgi";
            intrepreter[".py"] = (char*)"/usr/bin/python3";
            std::string filePath  = getFilePath(client);
            char *Path[3] = {intrepreter[client.response.GetFileExtention()], (char *)filePath.c_str(), NULL};
            fd = open (outfile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
            if (fd < 0)
                throw(std::runtime_error("Open Failed"));
            dup2(fd, 1);
            close(fd);
            execve(Path[0], Path, 0); // ! env itzad
            std::cout << "ERRRRORRR" << std::endl;
            exit(EXFIALE);
        }
        waitpid(pid, 0, 0);
        fd = open (outfile.c_str(), O_CREAT | O_RDWR , 0777);
        if (fd < 0)
            throw(std::runtime_error("Open Failed"));
        // std::cout << "read  bytes : " << read(fd, buff, 10) << std::endl;;
    }
    else
    {
        
        fd = open (getFilePath(client).c_str(), O_RDONLY);
        if (fd < 0)
            throw(std::runtime_error("Open Failed"));
    }
    fstat(fd, &statbuffer);
    ss << statbuffer.st_size;
    // std::cout << "out size " << ss.str() << std::endl;
    header = client.response.getHttpVersion() + " 200 OK\r\nContent-Type: "
    + client.response.getContentType() + "\r\ncontent-length: " + ss.str() + "\r\n\r\n";
    std::cout << header << std::endl;
    send(client.GetSocketId(), header.c_str(), header.size(), 0);
    while (read(fd, buff, BUFFER_SIZE) > 0)
        if (write (client.GetSocketId() , buff, BUFFER_SIZE) < 0)
            throw(std::runtime_error("Write buff in Get Failed"));
    sleep(2);
    close (fd);
}


void    ft_Response(const Client &client)
{
    try
    {
        // std::cout << "FILE : " << getFilePath(client).c_str() << std::endl;
        Get(client);
        // exit(0);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

// int main()
// {
//     std::cout << "helll oworld" << std::endl;
// }

// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <cstring>


// int main() {

//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);

//     // Create socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("Socket creation failed");
//         return 1;
//     }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     // Bind the socket to the specified address and port
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         perror("Bind failed");
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(server_fd, 3) < 0) {
//         perror("Listen failed");
//         return 1;
//     }

//     // std::cout << "Listening on port " << PORT << "..." << std::endl;

//     // Accept a new connection
//     if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
//         perror("Accept failed");
//         return 1;
//     }
//     try
//     {
//         ft_Response(Client("GET" , "test.py", ".py",  "text/plain", "HTTP/1.1 ", new_socket));
//     }
//     catch(std::exception &e){
//         std::cout << "Error : " << e.what() << std::endl;
//     }
// }