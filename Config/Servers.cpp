/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:11:31 by aybiouss          #+#    #+#             */
/*   Updated: 2023/10/03 23:59:52 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Servers.hpp"
// #include "../Response/Response.hpp"

// Getting all the blocks !!!
Servers::Servers() {
    // _client.clear();
    // _servers.clear();
}

Servers::Servers(const Servers& other)
{
    *this = other;
}

Servers& Servers::operator=(const Servers& other)
{
    if (this != &other)
    {
        _servers = other._servers;
        _client = other._client;
    }
    return *this;
}


int Servers::ConfigFileParse(std::string file)
{
    std::ifstream File(file.c_str());
    if (!File.is_open())
    {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        return 1;
    }

    std::string line;
    bool insideServerBlock = false;
    std::vector<std::string> block;
    std::stack<char> blockStack; // Stack to keep track of nested blocks
    while (std::getline(File, line))
    {
        if (line == "server")
        {
            insideServerBlock = true;
            continue; // Skip the "server" line
        }
        if (insideServerBlock)
        {
            if (line == "{")
            {
                blockStack.push('{'); // Push a '{' for nested blocks
                continue;             // Skip the opening curly brace
            }
            else if (line == "}")
            {
                blockStack.pop(); // Pop a '{' to close the current block
                if (blockStack.empty())
                {
                    insideServerBlock = false; // Stop when we encounter the closing curly brace
                    // hna ndwz string kamlo (block) bach itparsa f configuration onmchi n9lb 3la next server f configfile
                    Configuration config(block);
                    _servers.push_back(config);
                    std::cout  << "Servers size : " << _servers.size() << std::endl;
                    //     std::cout << block[i] << std::endl;
                    // }
                    block.clear(); // Clear the block for the next server
                }
            }
        }
        bool nospace = false;
        for (size_t i = 0; i < line.length(); ++i)
        {
            if (!std::isspace(static_cast<unsigned char>(line[i])))
            {
                nospace = true;
            }
        }
        if (nospace)
            block.push_back(line); // If any non-whitespace character is found
    }
    File.close();
    // printServerData();
    if (_servers.size() > 1)
        checkServers();
    AllServers();
    return 0;
}

void Servers::checkServers()
{
    std::vector<Configuration>::iterator it1;
    std::vector<Configuration>::iterator it2;
    for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
    {
        for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
        {
            if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerNames() == it2->getServerNames())
                throw std::string("Failed server validation");
        }
    }
}

void Servers::printServerData() const
{
    for (std::vector<Configuration>::const_iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

int Servers::AllServers()
{
    int maxFd = 0;   // will store the maximum file descriptor value for use in select()
    fd_set read_fds; // fd_set is a data structure used to manage file descriptors for I/O operations.
                     //  Fill up a fd_set structure with the file descriptors you want to know when data comes in on.
    int server_fd;
    fd_set write_fds;
    Socket sockets;
    int yes = 1;
    std::vector<int> clientsocket;
    std::map<int, Configuration> serverSockets;
    int i(10);
    for (std::vector<Configuration>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        struct addrinfo hints, *p, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        // hints.ai_flags = AI_PASSIVE; // !
        std::ostringstream s;
        s << it->getPort();
        if (getaddrinfo(it->getHost().c_str(), s.str().c_str(), &hints, &res) != 0)
        {
            std::cerr << "Error resolving hostname: " << it->getHost() << std::endl;
            continue;
        }
        for (p = res; p != NULL; p = p->ai_next)
        {
            if ((server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("server: socket");
                continue;
            }
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            {
                perror("setsockopt");
                exit(1);
            }
            if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1)
            {
                close(server_fd);
                perror("server: bind");
                continue;
            }
            break;
        }
        freeaddrinfo(res);
        if (p == NULL)
        {
            fprintf(stderr, "server: failed to bind\n");
            exit(1);
        }
        sockets.setnonblocking(&server_fd); // !
        if (listen(server_fd, MAX_CLIENTS) < 0)
        {
            perror("Listen failed");
            // fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
            exit(EXIT_FAILURE);
        } // listens for incoming connections on the server socket (server_fd)
        std::cout << "Listening on port " << it->getPort() << std::endl;
        if (server_fd > maxFd)
            maxFd = server_fd;
        serverSockets[server_fd] = *it;
        i++;
    }
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    for (std::map<int, Configuration>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
    {
        if (it->first >= 0) {
            FD_SET(it->first, &read_fds);
        }
        else {
            std::cout << "FD_SET fails To add the it->first to read_fds" << std::endl;
            exit(20);
        }
    }
    while (true)
    {
        fd_set tmp_read = read_fds;
        fd_set tmp_write = write_fds;
        int readySockets = select(maxFd + 1, &tmp_read, &tmp_write, NULL, NULL); // !
        std::cout << "__________under Select__________" << std::endl;
        if (readySockets < 0)
        {
            for (int fd = 0; fd <= maxFd; fd++)
            {
                if (FD_ISSET(fd, &tmp_read) || FD_ISSET(fd, &tmp_write))
                {
                    std::cerr << "Problematic FD: " << fd << std::endl;
                }
            }
            perror("Error with select");
            sleep(2);
            exit(EXIT_FAILURE);
        }
        for (std::map<int, Configuration>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
        {
            // std::cout << "loop over server sockets" << std::endl;
            if (FD_ISSET(it->first, &tmp_read))
            {
                Client new_client;
                sockaddr_in clientAddr;
                int clientSocketw;
                socklen_t addrlen = sizeof(clientAddr);
                if ((clientSocketw = accept(it->first, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen)) < 0) // is used to accept this incoming connection. It creates a new socket descriptor (new_socket) for this specific client connection. The client's address information is stored in address.
                {
                    perror("Error accepting connection");
                    continue;
                }
                std::cout << "\e[1;32m" << "ps: " << it->first << " accept new connection\e[0m: " << clientSocketw << std::endl;
                if (clientSocketw > maxFd)
                    maxFd = clientSocketw;
                new_client.set_socket(clientSocketw);
                new_client.set_server(it->second);
                _client.push_back(new_client);
                if (clientSocketw >= 0) {
                    std::cout << "add " << clientSocketw << " to the read_fds" << std::endl;
                    FD_SET(clientSocketw, &read_fds);
                }
                else {
                   std::cout << "FD_SET fails To add the clientSocketw to read_fds" << std::endl;
                   exit(20);
                }

            }
        }
        for (std::vector<Client>::iterator its = _client.begin(); its != _client.end(); its++)
        {
            if (FD_ISSET(its->GetSocketId(), &tmp_read))
            {
                // std::cout << "\e[1;34mreading request [client socket: " << its->GetSocketId() << "]\e[0m" << std::endl;
                char buffer[1024] = {0};
                // Read the HTTP request from the client
                ssize_t bytesRead = recv(its->GetSocketId(), buffer, sizeof(buffer) - 1, 0);
                // std::string s(buffer, bytesRead);
                if (bytesRead < 0)
                {
                    perror("Error reading from socket");
                    close(its->GetSocketId());
                    if (its->GetSocketId() == maxFd)
                        maxFd -= 1;
                    its = _client.erase(its);
                    its--;
                    FD_CLR(its->GetSocketId(), &read_fds);
                    continue;
                }
                else if (bytesRead == 0)
                {
                    close(its->GetSocketId());
                    if (its->GetSocketId() == maxFd)
                        maxFd -= 1;
                    FD_CLR(its->GetSocketId(), &read_fds);
                    its = _client.erase(its);
                    its--;
                    continue;
                }
                else
                {
                    std::string buf(buffer, bytesRead);
                    std::cout << "psps" << std::endl;
                    if (!its->response.parseHttpRequest(buf, its->GetSocketId(), bytesRead)) // la 9ra kolchi
                    {
                        FD_CLR(its->GetSocketId(), &read_fds);
                        std::cout << "add " << its->GetSocketId() << " to write_fds " << std::endl;
                        FD_SET(its->GetSocketId(), &write_fds);
                    }
                }
            }
        }
        for (std::vector<Client>::iterator its = _client.begin(); its != _client.end();)
        {
            if (FD_ISSET(its->GetSocketId(), &tmp_write))
            {
                std::cout << "1: SOCKET ID " << its->GetSocketId() << std::endl;
                its->_readStatus = -2;
                if (its->_status == 0)
                    ft_Response(*its);
                else
                    ft_send(*its);
                if (its->_readStatus == -1 || its->_readStatus == 0)
                {
                    FD_CLR(its->GetSocketId(), &write_fds);
                    close(its->GetSocketId());
                    close(its->_content_fd);
                    its->set_socket(-1);
                    its->_content_fd = -1;
            std::cout << "2: SOCKET ID " << its->GetSocketId() << std::endl;
                    its = _client.erase(its);
                }
            }
            else
                ++its;
        }
    }
    return 0;
}
// for (std::vector<Client>::iterator it = _client.begin(); it != _client.end(); it++)
// {
//     if (FD_ISSET(it->GetSocketId(), &tmp_write))
//     {
        
//     }
// }