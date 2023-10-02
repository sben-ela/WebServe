/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
/*   Updated: 2023/10/01 21:33:52 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Webserv.hpp"
#include "../Includes/Configuration.hpp"
#include "../Includes/Client.hpp"


/// @brief  create a random file name in the tmp dir
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
                        std::stringstream ss;
                        ss << fileStat.st_size;
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

/// @brief get the requested path without the location name
std::string getFileName(const std::string& path, size_t first)
{

    std::string fileName = path.substr(first);

    return("/" + fileName);
}

/// @brief join root with the requested path 
std::string     getFilePath(const Client& client)
{
    std::string filePath;
	size_t index;

	index = getLocationIndex(client);
        
	filePath = client.getServer().getRoot() + getFileName(client.response.getPath()
	, client.getServer().getLocations()[index].getpattern().size());
	return (filePath);
}

void    SendErrorPage(Client &client, int errorNumber)
{
    std::stringstream ss;
    struct stat statbuffer;
    char buff[BUFFER_SIZE];
    std::string header;

    int efd = open(client.getServer().getErrorPages()[errorNumber].c_str(), O_RDONLY);
    if (efd < 0)
        throw(std::runtime_error("Invalid Error page !"));
    fstat(efd, &statbuffer);
    ss << statbuffer.st_size;
    if (errorNumber == MOVEDPERMANENTLY)
        header = client.response.getHttpVersion() + client.response.getStatusCode()[errorNumber] + "\r\nContent-Length: " + ss.str() + "\r\nLocation: " + client.response.getPath() + "/" + "\r\n\r\n";
    else
        header = client.response.getHttpVersion() + client.response.getStatusCode()[errorNumber] + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
    int bytes = write(client.GetSocketId(), header.c_str(), header.size());
    if (bytes < 0)
        throw(std::runtime_error(" write failed in sendErrorpage"));
    int rd = read(efd, buff, BUFFER_SIZE);
    buff[rd] = '\0';
    write(client.GetSocketId(), buff, BUFFER_SIZE);
    client._readStatus = -1;
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
    return ("text/plain");
}

std::string getExtention(const std::string& filePath)
{
    size_t dotIndex;

    dotIndex = std::min(filePath.find('.'), filePath.size());
    return(filePath.substr(dotIndex));
}

void ft_send(Client& client)
{
    char buff[BUFFER_SIZE];

    if ((client._readStatus = read(client._content_fd, buff, BUFFER_SIZE)) > 0)
    {
        std::cout << "ID : " << client.GetSocketId() << std::endl;
        if (write(client.GetSocketId() , buff, BUFFER_SIZE) < 0)
        {
            client._readStatus = -1;
            perror("write in ft_send Faild ");
            exit(1);
        }
    }
    if (client._readStatus < 0)
    {
        perror("read in ft_send Faild ");
        exit(1);
    }
}

/// @brief GET method
void    Get(Client &client)
{
    char buff[BUFFER_SIZE];
    std::string header;
    std::stringstream ss;
    struct stat statbuffer;
    int readBytes = 0;
    int bodyFd;
    int fd;
    memset(buff, 0, BUFFER_SIZE);
    if (client.response.GetFileExtention() == ".php" || client.response.GetFileExtention() == ".py")
    {
        std::string outfile = GenerateFile();
        int pid  = fork();
        if (!pid)
        {
            std::map<std::string, std::string> intrepreter = client.getServer().getCgi();
            std::string filePath  = getFilePath(client);
            char *Path[3] = {(char*)intrepreter[client.response.GetFileExtention()].c_str(), (char *)filePath.c_str(), NULL};
            fd = open (outfile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
            if (fd < 0)
                throw(std::runtime_error("Open Failed"));
            dup2(fd, 1);
            close(fd);
            if (client.response.getMethod() == "POST")
            {
                bodyFd = client.response.getFd();
                dup2(bodyFd, 0);
                close(bodyFd);
            }
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
    client._content_fd = fd;
    client._status = 1;
}

bool file_exists(const std::string &filename)
{
    struct stat buffer;

    return(stat(filename.c_str(), &buffer) == 0);
}

bool isDirectory(const char* path) {
    struct stat fileInfo;

    if (stat(path, &fileInfo) != 0)
        throw(std::runtime_error("stat failed in isDirectory"));

    return S_ISDIR(fileInfo.st_mode);
}


void    DirectoryHasIndexFile(Client &client, const std::string& indexFile)
{
    client.response.setPath(client.response.getPath() + indexFile);
    if (file_exists(client.response.getPath())) // ! protect invalid index file
        Get(client);
    else
        SendErrorPage(client, NOTFOUND); 
}

/// @brief if the request is a directory 
void    handleDirectory(Client &client, const std::string& filePath)
{
    size_t locationIndex = getLocationIndex(client);
    if (filePath[filePath.size() - 1] != '/')
        SendErrorPage(client, MOVEDPERMANENTLY);
    if (client.response.getMethod() == "GET" && client.getServer().getLocations()[locationIndex].getIndex().empty() && client.getServer().getIndex().empty())
    {
        if (client.getServer().getLocations()[locationIndex].getAutoIndex())
        {
            std::stringstream ss;
            std::string test = GenerateDirectoryListing(filePath);
            ss << test.size();
            std::string header = client.response.getHttpVersion() + " 200 OK\r\nContent-Type: "
                + "text/html" + "\r\nContent-length: " + ss.str() + "\r\n\r\n";
            if (write(client.GetSocketId(), header.c_str(), header.size()) < 0)
            {
                perror("write in handleDirectory Faild : ");
                exit(13);
            }
            if (write(client.GetSocketId(), test.c_str(), test.size()) < 0)
            {
                perror("write in handleDirectory Faild : ");
                exit(37);
            }
            client._readStatus = -1;
        }
        else
            SendErrorPage(client, FORBIDDEN);
    }
    else if (!client.getServer().getLocations()[locationIndex].getIndex().empty())
        DirectoryHasIndexFile(client, client.getServer().getLocations()[locationIndex].getIndex());
    else if (!client.getServer().getIndex().empty())
        DirectoryHasIndexFile(client, client.getServer().getIndex());
    else
    {
        std::cout << "Forbeddin\n";
        SendErrorPage(client, FORBIDDEN);
    }
}

/// @brief Initialize methods with their state
void initMethods(Methods& methods, std::vector<std::string> allowMethods)
{
    for (size_t i = 0; i < allowMethods.size(); i++)
    {
        if (allowMethods[i] == "GET")
            methods._get = true;
        else if (allowMethods[i] == "POST")
            methods._post = true;
        else if (allowMethods[i] == "DELETE")
            methods._delete = true;
    }
}

void    checkIndexFile(Client client, const std::string& indexFile, const std::string& targetPath)
{
    if (getExtention(indexFile) != ".php" && getExtention(indexFile) != ".py")
        Delete_dir(targetPath);
    else
        DirectoryHasIndexFile(client, indexFile);
}

/// @brief delete directory
void    Delete_dir(const std::string& folderPath)
{
    DIR* dir;
    struct dirent* target;

    if ((dir = opendir(folderPath.c_str())) != NULL)
    {
        while ((target = readdir(dir)) != NULL)
        {
            if (target->d_type == DT_REG)
                std::remove((folderPath + "/" + target->d_name).c_str());
            else if (strcmp( target->d_name , ".") && strcmp( target->d_name , "..") && target->d_type == DT_DIR)
            {
                Delete_dir(folderPath + "/" + target->d_name);
                rmdir((folderPath + "/" + target->d_name).c_str());
            }
            
        }
        rmdir(folderPath.c_str());
    }
    free(dir);
}

/// @brief DELETE method 
void    ft_delete(Client &client)
{
    std::string targetPath = getFilePath(client);
    size_t locationIndex = getLocationIndex(client);
    if (isDirectory(targetPath.c_str()))
    {
        if (targetPath[targetPath.size() - 1] != '/')
            SendErrorPage(client, CONFLICT);
        else if(client.getServer().getLocations()[locationIndex].getIndex().empty() && client.getServer().getIndex().empty())
            Delete_dir(targetPath);
        else if (!client.getServer().getLocations()[locationIndex].getIndex().empty())
            checkIndexFile(client, client.getServer().getLocations()[locationIndex].getIndex(), targetPath);
        else if (!client.getServer().getIndex().empty())
            checkIndexFile(client, client.getServer().getIndex(), targetPath);
    }
    else
    {
        std::remove(targetPath.c_str());
    }
    std::cout << " Target : " << targetPath << std::endl;
}


void    ft_Response(Client &client)
{
    try
    {
        std::cout << "********************START-RESPONSE  : " << "*******************" << std::endl;
        client.response.CreateStatusCode();
        // std::cout << "Rs satatus : " << client.response.getResponseStatus() << std::endl;
        // if (client.response.getResponseStatus() != 0)
        //     SendErrorPage(client, client.response.getResponseStatus());
        std::string filePath = getFilePath(client).c_str();
        short index = getLocationIndex(client);
        initMethods(client.methods, client.getServer().getLocations()[index].getLimit_except());
        if (access(filePath.c_str(), F_OK))
            SendErrorPage(client, NOTFOUND);
        else if (access(filePath.c_str(), R_OK))
            SendErrorPage(client, FORBIDDEN);
        else if (client.response.getMethod() == "GET")
        {
            if (!client.methods._get)
                SendErrorPage(client, FORBIDDEN);
            if (isDirectory(filePath.c_str()))
                handleDirectory(client, filePath);
            else
                Get(client);
        }
        else if (client.response.getMethod() == "DELETE")
        {
            if (!client.methods._delete)
                SendErrorPage(client, FORBIDDEN);
            ft_delete(client);
        }
        else if (client.response.getMethod() == "POST")
        {
            if (!client.methods._post)
                SendErrorPage(client, FORBIDDEN);
            if (client.getServer().getLocations()[index].getUpload().empty())
                throw(std::runtime_error("empty upload path"));
            if (isDirectory(filePath.c_str()))
                handleDirectory(client, filePath);
            else
                Get(client);
            // SendErrorPage(client, CREATED);
        }
    }
    catch(std::exception &e)
    {
        client._readStatus = -1;
        std::cout << "What...?" << std::endl;
        std::cout << e.what() << std::endl;
    }
    std::cout << "********************END-RESPONSE*******************" << std::endl;
}
