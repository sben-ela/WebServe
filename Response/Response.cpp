/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
/*   Updated: 2023/09/25 23:39:12 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Webserv.hpp"
#include "../Includes/Configuration.hpp"
#include "../Includes/Client.hpp"


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


void ft_putstr(char *str, int size)
{
    int i = 0;
    while (i < size)
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


std::string GenerateDirectoryListing(const std::string& directoryPath) {
    std::string html;
    html += "<html><head><title>Directory Listing</title></head><body>";
    html += "<h1>Directory Listing</h1>";
    html += "<table border='1'><tr><th>Name</th><th>Size</th><th>Date Modified</th></tr>";
    std::stringstream ss;
    
    // Open the directory
    DIR* dir = opendir(directoryPath.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
                // Get file information
                std::string filePath = directoryPath + "/" + entry->d_name;
                struct stat fileStat;
                if (stat(filePath.c_str(), &fileStat) == 0) {
                    // Format file size
                    std::string fileSize;
                    if (S_ISDIR(fileStat.st_mode)) {
                        fileSize = "Directory";
                    } else {
                        ss >> fileStat.st_size;
                        fileSize = ss.str() + " bytes";
                    }

                    // Format date modified
                    char dateModified[100];
                    strftime(dateModified, sizeof(dateModified), "%D, %r", localtime(&fileStat.st_mtime));

                    // Add row to the table
                    html += "<tr><td><a href='" + std::string(entry->d_name) + "'>" + entry->d_name + "</a></td><td>" + fileSize + "</td><td>" + dateModified + "</td></tr>";
                }
            }
        }
        closedir(dir);
    }

    html += "</table></body></html>";
    return html;
}

std::string getFileName(const std::string& path, size_t first)
{
    std::string fileName = path.substr(first);

    return("/" + fileName);
}

std::string     getFilePath(const Client& client)
{
    std::string filePath;
	size_t index;

	index = getLocationIndex(client);
        
	filePath = client.getServer().getRoot() + getFileName(client.response.getPath()
	, client.getServer().getLocations()[index].getpattern().size());
	return (filePath);
}

void    SendErrorPage(Client client, int errorNumber)
{

    std::stringstream ss;
    struct stat statbuffer;
    char buff[BUFFER_SIZE];

    int efd = open(client.getServer().getErrorPages()[errorNumber].c_str(), O_RDONLY);
    if (efd < 0)
        throw(std::runtime_error("open Failed in SendErrorPage !"));
    fstat(efd, &statbuffer);
    ss << statbuffer.st_size;
    std::string header = client.response.getHttpVersion() + client.getServer().getErrorPages()[errorNumber]+ "\r\ncontent-length: " + ss.str() + "\r\n\r\n";
    write(client.GetSocketId(), header.c_str(), header.size());
    int rd = read(efd, buff, BUFFER_SIZE);
    buff[rd] = '\0';
    write(client.GetSocketId(), buff, BUFFER_SIZE);
}

const char *get_content_type(const char* path)
{
    const char *last_dot = strrchr(path, '.');
    if (last_dot)
    {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".mp4") == 0) return "video/mp4";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
        if (strcmp(last_dot, ".py") == 0) return "text/plain";
        if (strcmp(last_dot, ".php") == 0) return "text/plain";
    }
    return ("application/octet-stream");
}
void    Get(const Client &client)
{
    char buff[BUFFER_SIZE];
    std::string header;
    std::stringstream ss;
    struct stat statbuffer;
    int readBytes = 0;
    int fd;

    memset(buff, 0, BUFFER_SIZE);
    if (client.response.GetFileExtention() == ".php" || client.response.GetFileExtention() == ".py")
    {
        std::string outfile = GenerateFile();
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
            throw(std::runtime_error("Open Failed  in GET-CGI "));
        if (client.response.GetFileExtention() == ".php")
            readBytes = read(fd, buff, 42);
    }
    else
    {
        fd = open (getFilePath(client).c_str(), O_RDONLY);
        if (fd < 0)
            throw(std::runtime_error("Open Failed in GgI"));
    }
    fstat(fd, &statbuffer);
    ss << statbuffer.st_size - readBytes;
    header = client.response.getHttpVersion() + " 200 OK\r\nContent-Type: "
    + get_content_type(client.response.getPath().c_str()) + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
    send(client.GetSocketId(), header.c_str(), header.size(), 0);
    while (read(fd, buff, BUFFER_SIZE) > 0)
    {
        if (write(client.GetSocketId() , buff, BUFFER_SIZE) < 0)
            throw(std::runtime_error("Write buff in Get Failed"));
    }
    close (fd);
}

bool isDirectory(const char* path) {
    struct stat fileInfo;

    if (stat(path, &fileInfo) != 0)
        throw(std::runtime_error("stat failed in isDirectory"));

    return S_ISDIR(fileInfo.st_mode);
}

void    DirectoryHasIndexFile(Client client, const std::string& indexFile)
{
    client.response.setPAth(client.response.getPath() + indexFile);
    Get(client);
}

void    handleDirectory(Client &client, const std::string& filePath)
{
    size_t locationIndex = getLocationIndex(client);
    if (filePath[filePath.size() - 1] != '/')
        SendErrorPage(client, MOVEDPERMANENTLY);
    else
    {
        if (client.getServer().getLocations()[locationIndex].getIndex().empty() && client.getServer().getIndex().empty())
        {
            if (client.getServer().getLocations()[locationIndex].getAutoIndex())
            {
                std::stringstream ss;
                std::string test = GenerateDirectoryListing(filePath);
                ss << test.size();
                std::string header = client.response.getHttpVersion() + " 200 OK\r\nContent-Type: "
                    + "text/html" + "\r\nContent-length: " + ss.str() + "\r\n\r\n";
                write(client.GetSocketId(), header.c_str(), header.size());
                write(client.GetSocketId(), test.c_str(), test.size());
            }
            else
                SendErrorPage(client, 403);
        }
        else if (!client.getServer().getLocations()[locationIndex].getIndex().empty()) // ! index khaso i3mer osf 
            DirectoryHasIndexFile(client, client.getServer().getLocations()[locationIndex].getIndex());
        else if (!client.getServer().getIndex().empty())
            DirectoryHasIndexFile(client, client.getServer().getIndex());
    }
}

void    ft_Response(Client &client)
{
    try
    {
        
        std::string filePath = getFilePath(client).c_str();
    
        if (access(filePath.c_str(), F_OK))
            SendErrorPage(client, 404);
        else if (access(filePath.c_str(), R_OK))
            SendErrorPage(client, 403);
        if (client.response.getMethod() == "GET")
        {
            if (isDirectory(filePath.c_str()))
                handleDirectory(client, filePath);
            else
                Get(client);
        }
        // else if (client.response.getMethod() == "POST")
        //     ;
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "###################################################" << std::endl;
        // exit(0);
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