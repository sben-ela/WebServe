// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Response.cpp                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
// /*   Updated: 2023/09/21 10:50:54 by sben-ela         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "Response.hpp"

// #define FILESIZE 10
// #define EXFIALE 13 

// std::string GenerateFile( void )
// {
//     std::string Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefjhiGklmnopqrstuvwxyz+-_";
//     std::string file;
    
//     for (size_t i = 0; i < FILESIZE; i++)
//     {
//         file += Base[rand() % Base.size()];
//     }
//     return(file);
// }

// void    SeparatesHeader(std::string& str)
// {
//     str = str.substr(str.find("\r\n\r\n") + 4);
// }

// void	CgiExecute(int ofd, const Client &client)
// {
// 	int fd[2];
//     std::string output;

//     std::map<std::string, char*> intrepreter;
//     intrepreter[".php"] = (char *)"/usr/bin/php-cgi";
//     intrepreter[".py"] = (char*)"/usr/bin/python3";
// 	char *Path[3] = {intrepreter[client.GetFileExtention()], (char *)client.GetPath().c_str(), NULL};

// 	pipe(fd);
// 	int pid  = fork();
// 	if (!pid)
// 	{
//         std::cout << "in child" << ofd << std::endl;
//         exit(0);
//         dup2(ofd, 1);
// 		execve(Path[0], Path, 0);
// 		std::cout << "ERRRRORRR" << std::endl;
//         exit(EXFIALE);
// 	}
// 	waitpid(pid, 0, 0);
//     close(fd[1]);
//     // ssize_t bytesRead;
//     // char buffer[1024];
//     // while ((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0)
//     //     output.append(buffer, bytesRead);
//     // if (client.GetFileExtention() == ".php")
//     //     SeparatesHeader(output);
// 	// return(output);
// }


// void ft_putstr(char *str)
// {
//     int i = 0;
//     while (i < BUFFER_SIZE)
//     {
//         if (write (1 , &str[i], 1) < 0)
//         {
//             std::cout << "putstr failed : " << str[i] << std::endl;
//             exit (0);
//         }
//         i++;
//     }
// }

// void    Get(const Client &client)
// {
//     char buff[BUFFER_SIZE];
//     std::string header;
//     int fd;
// 	// int pfd[2];
    
//     if (client.GetFileExtention() == ".php" || client.GetFileExtention() == ".py")
//     {
//         // std::string filename = GenerateFile();
//         fd = open("test" , O_RDWR);
//         if (fd < 0)
//             throw(std::runtime_error("bad file"));
//         std::cout << fd  << std::endl;;
//         // CgiExecute(fd, client);
//         std::map<std::string, char*> intrepreter;
//         intrepreter[".php"] = (char *)"/usr/bin/php-cgi";
//         intrepreter[".py"] = (char*)"/usr/bin/python3";
//         char *Path[3] = {intrepreter[client.GetFileExtention()], (char *)client.GetPath().c_str(), NULL};

//         // pipe(pfd);
//         int pid  = fork();
//         if (!pid)
//         {
//             dup2(fd, 1);
//             execve(Path[0], Path, 0);
//             std::cout << "ERRRRORRR" << std::endl;
//             exit(EXFIALE);
//         }
//         waitpid(pid, 0, 0);
//         // close(fd);
        
//     }
//     else
//     {
//         fd = open (client.GetPath().c_str(), O_RDONLY);
//         if (fd < 0)
//             throw(std::runtime_error("open Path in Get failed"));
//     }
//     std::stringstream ss;
//     struct stat statbuffer;
//     fstat(fd, &statbuffer);
//     ss << statbuffer.st_size;
//     std::cout << ss.str() << std::endl;
//     header = client.GetHttpVersion() + " 200 OK\r\nContent-Type: "
//     + client.GetConetType() + "\r\ncontent-length: 45" + "\r\n\r\n"; 

//     send(client.GetSocketId(), header.c_str(), header.size(), 0);
//     int rd = 0;
//     // rd = read(fd, buff, BUFFER_SIZE);
//     // while (read(fd, buff, BUFFER_SIZE) > 0)
//         rd = write (client.GetSocketId() , "hell ofdvgjdgvyudfgukfdgvjkdfdfvjhdfvjhdfgdf", 45);
//         // if (rd = write (client.GetSocketId() , buff, BUFFER_SIZE) < 0)
//         //     throw(std::runtime_error("Write buff in Get Failed"));
//     close (fd);
//     std::cout << rd << std::endl;;
// }



// // void    Post(const Client& client, std::string Content)
// // {11

// // }

// void    ft_Response(const Client &client)
// {
//     std::string responseStr;

//     if (client.GetMethod() == "GET")
//         Get(client);
//     // else if (client.GetMethod() == "POST")
//     //     Post();
//     // else if (client.GetMethod() == "DELETE")
//     //     Delete();
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
/*   Updated: 2023/09/20 21:52:26 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#define FILESIZE 10
#define EXFIALE 13 


void    SeparatesHeader(std::string& str)
{
    str = str.substr(str.find("\r\n\r\n") + 4);
}

std::string	CgiExecute(const Client &client)
{
	int fd[2];
    std::string output;

    std::map<std::string, char*> intrepreter;
    intrepreter[".php"] = (char *)"/usr/bin/php-cgi";
    intrepreter[".py"] = (char*)"/usr/bin/python3";
	char *Path[3] = {intrepreter[client.GetFileExtention()], (char *)client.GetPath().c_str(), NULL};

	pipe(fd);
	int pid  = fork();
	if (!pid)
	{
		dup2(fd[1], 1);
		execve(Path[0], Path, 0);
		std::cout << "ERRRRORRR" << std::endl;
        exit(EXFIALE);
	}
	waitpid(pid, 0, 0);
    close(fd[1]);
    ssize_t bytesRead;
    char buffer[1024];
    while ((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0)
        output.append(buffer, bytesRead);
    if (client.GetFileExtention() == ".php")
        SeparatesHeader(output);
	return(output);
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

void    Get(const Client &client)
{
    char buff[BUFFER_SIZE];
    std::string header;
    std::stringstream ss;
    struct stat statbuffer;
    int fd;



    if (client.GetFileExtention() == ".php" || client.GetFileExtention() == ".py")
    {
        std::map<std::string, char*> intrepreter;
        intrepreter[".php"] = (char *)"/usr/bin/php-cgi";
        intrepreter[".py"] = (char*)"/usr/bin/python3";
        char *Path[3] = {intrepreter[client.GetFileExtention()], (char *)client.GetPath().c_str(), NULL};

        fd = open ("test", O_CREAT | O_RDWR | O_TRUNC, 0777);
        int pid  = fork();
        if (!pid)
        {
            dup2(fd, 1);
            close(fd);
            execve(Path[0], Path, 0);
            std::cout << "ERRRRORRR" << std::endl;
            exit(EXFIALE);
        }
        waitpid(pid, 0, 0);
        close(fd);
        fd = open ("test", O_CREAT | O_RDWR, 0777);
    }
    else
    {
        fd = open (client.GetPath().c_str(), O_RDONLY);
        if (fd < 0)
            throw(std::runtime_error("open Path in Get failed"));
    }
    fstat(fd, &statbuffer);
    ss << statbuffer.st_size;
    std::cout << "out size " << ss.str() << std::endl;
    header = client.GetHttpVersion() + " 200 OK\r\nContent-Type: "
    + client.GetConetType() + "\r\ncontent-length: " + ss.str() + "\r\n\r\n";

    send(client.GetSocketId(), header.c_str(), header.size(), 0);
    // std::cout << "pfd : " << fd << std::endl;
    // std::cout << "read " << read(fd, buff, BUFFER_SIZE) << std::endl;
    while (read(fd, buff, BUFFER_SIZE) > 0)
        if (write (client.GetSocketId() , buff, BUFFER_SIZE) < 0)
            throw(std::runtime_error("Write buff in Get Failed"));
    close (fd);
}

std::string GenerateFile( void )
{
    std::string Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefjhiGklmnopqrstuvwxyz+-_";
    std::string file;
    
    for (size_t i = 0; i < FILESIZE; i++)
    {
        file += Base[rand() % Base.size()];
    }
    return(file);
}



void    ft_Response(const Client &client)
{
    std::string responseStr;

    if (client.GetMethod() == "GET")
        Get(client);
    // else if (client.GetMethod() == "POST")
    //     Post();
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
    try
    {
        ft_Response(Client("GET" , "test.py", ".py",  "text/plain", "HTTP/1.1 ", new_socket));
    }
    catch(std::exception &e){
        std::cout << "Error : " << e.what() << std::endl;
    }
}