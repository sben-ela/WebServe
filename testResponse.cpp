#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <sys/select.h>

// Handle incoming connections
void handleConnection(int clientSocket, int serverPort)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Read the HTTP request from the client
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead < 0)
    {
        std::cerr << "Error reading from socket on port " << serverPort << std::endl;
        return;
    }

    // Implement your request handling logic here
    // You can parse the HTTP request, process it, and send a response

    // For demonstration, we'll just send a simple response
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, Port " + std::to_string(serverPort) + "!";
    send(clientSocket, response.c_str(), response.size(), 0);

    // Close the client socket
    close(clientSocket);
}

int main()
{
    // Define servers and their corresponding ports
    std::map<int, std::vector<int>> serverPorts;
    serverPorts[1] = {8080, 8081};
    serverPorts[2] = {9090};

    // Create and set up socket descriptors for each server and port
    std::map<int, std::vector<int>> serverSockets;
    fd_set readSet;
    int maxFd = 0;

    for (const auto& [serverId, ports] : serverPorts)
    {
        for (int port : ports)
        {
            int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket == -1)
            {
                std::cerr << "Error creating socket for port " << port << " on server " << serverId << std::endl;
                continue;
            }

            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            serverAddr.sin_addr.s_addr = INADDR_ANY;

            if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
            {
                std::cerr << "Error binding socket for port " << port << " on server " << serverId << std::endl;
                close(serverSocket);
                continue;
            }

            if (listen(serverSocket, 10) == -1)
            {
                std::cerr << "Error listening on port " << port << " on server " << serverId << std::endl;
                close(serverSocket);
                continue;
            }

            std::cout << "Server " << serverId << " is listening on port " << port << std::endl;

            serverSockets[serverId].push_back(serverSocket);

            if (serverSocket > maxFd)
            {
                maxFd = serverSocket;
            }
        }
    }

    // Main event loop using select
    while (true)
    {
        FD_ZERO(&readSet);
        for (const auto& [serverId, sockets] : serverSockets)
        {
            for (int serverSocket : sockets)
            {
                FD_SET(serverSocket, &readSet);
            }
        }

        int readySockets = select(maxFd + 1, &readSet, NULL, NULL, NULL);

        if (readySockets < 0)
        {
            std::cerr << "Error with select" << std::endl;
            break;
        }

        for (const auto& [serverId, sockets] : serverSockets)
        {
            for (int serverSocket : sockets)
            {
                if (FD_ISSET(serverSocket, &readSet))
                {
                    sockaddr_in clientAddr;
                    socklen_t clientAddrLen = sizeof(clientAddr);
                    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

                    if (clientSocket == -1)
                    {
                        std::cerr << "Error accepting connection on port " << ntohs(clientAddr.sin_port) << " on server " << serverId << std::endl;
                        continue;
                    }

                    handleConnection(clientSocket, ntohs(clientAddr.sin_port));
                }
            }
        }
    }

    // Close server sockets
    for (const auto& [serverId, sockets] : serverSockets)
    {
        for (int serverSocket : sockets)
        {
            close(serverSocket);
        }
    }

    return 0;
}


std::cout << "---------------------------------------------------" << std::endl;


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> // Include for getaddrinfo
#include <vector>
#include <map>
#include <sys/select.h>

// Function to handle incoming connections
void handleConnection(int clientSocket, int serverPort)
{
    // Handle client request
    // ...
}

int main()
{
    // Define servers and their corresponding hostnames and ports
    std::map<int, std::vector<std::pair<std::string, int>>> serverHostPorts;
    serverHostPorts[1] = {std::make_pair("example1.com", 8080), std::make_pair("example2.com", 8081)};
    serverHostPorts[2] = {std::make_pair("example3.com", 9090)};

    // Create and set up socket descriptors for each server and port
    std::map<int, std::vector<int>> serverSockets;
    fd_set readSet;
    int maxFd = 0;

    for (std::map<int, std::vector<std::pair<std::string, int>>>::const_iterator server = serverHostPorts.begin(); server != serverHostPorts.end(); server++)
    {
        for (std::vector<std::pair<std::string, int>>::const_iterator hostPort = server->second.begin(); hostPort != server->second.end(); hostPort++)
        {
            int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket == -1)
            {
                std::cerr << "Error creating socket for port " << hostPort->second << " on server " << server->first << std::endl;
                continue;
            }

            // Resolve the hostname to an IP address using getaddrinfo
            struct addrinfo hints, *res;
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET; // IPv4
            hints.ai_socktype = SOCK_STREAM;

            if (getaddrinfo(hostPort->first.c_str(), NULL, &hints, &res) != 0)
            {
                std::cerr << "Error resolving hostname: " << hostPort->first << std::endl;
                close(serverSocket);
                continue;
            }

            // Bind to the resolved IP address
            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(hostPort->second);
            serverAddr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;

            if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
            {
                std::cerr << "Error binding socket for port " << hostPort->second << " on server " << server->first << std::endl;
                close(serverSocket);
                continue;
            }

            if (listen(serverSocket, 10) == -1)
            {
                std::cerr << "Error listening on port " << hostPort->second << " on server " << server->first << std::endl;
                close(serverSocket);
                continue;
            }

            std::cout << "Server " << server->first << " is listening on port " << hostPort->second << " with hostname " << hostPort->first << std::endl;

            serverSockets[server->first].push_back(serverSocket);

            if (serverSocket > maxFd)
            {
                maxFd = serverSocket;
            }

            freeaddrinfo(res); // Free the addrinfo structure
        }
    }

    // Main event loop using select
    while (true)
    {
        FD_ZERO(&readSet);
        for (std::map<int, std::vector<int>>::iterator server = serverSockets.begin(); server != serverSockets.end(); server++)
        {
            for (std::vector<int>::iterator socket = server->second.begin(); socket != server->second.end(); socket++)
            {
                FD_SET(*socket, &readSet);
            }
        }

        int readySockets = select(maxFd + 1, &readSet, NULL, NULL, NULL);

        if (readySockets < 0)
        {
            std::cerr << "Error with select" << std::endl;
            break;
        }

        for (std::map<int, std::vector<int>>::iterator server = serverSockets.begin(); server != serverSockets.end(); server++)
        {
            for (std::vector<int>::iterator socket = server->second.begin(); socket != server->second.end(); socket++)
            {
                if (FD_ISSET(*socket, &readSet))
                {
                    sockaddr_in clientAddr;
                    socklen_t clientLen = sizeof(clientAddr);
                    int clientSocket = accept(*socket, (struct sockaddr *)&clientAddr, &clientLen);

                    if (clientSocket < 0)
                    {
                        std::cerr << "Error accepting client connection" << std::endl;
                        continue;
                    }

                    // Handle the client connection
                    handleConnection(clientSocket, ntohs(clientAddr.sin_port));

                    close(clientSocket);
                }
            }
        }
    }

    // Close server sockets and clean up
    for (std::map<int, std::vector<int>>::iterator server = serverSockets.begin(); server != serverSockets.end(); server++)
    {
        for (std::vector<int>::iterator socket = server->second.begin(); socket != server->second.end(); socket++)
        {
            close(*socket);
        }
    }

    return 0;
}
