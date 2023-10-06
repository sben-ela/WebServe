/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
/*   Updated: 2023/10/05 22:49:27 by sben-ela         ###   ########.fr       */
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

void    Client::initLocationIndex( void )
{
	_locationIndex = _client_server.getLocations().size() - 1;;
	while (_locationIndex > 0)
	{
		if (_client_server.getLocations()[_locationIndex].getpattern() == response.getPath().substr(0, _client_server.getLocations()[_locationIndex].getpattern().size()))
            break;
		_locationIndex--;
    }
}

size_t Client::getLocationIndex( void )
{
    return(_locationIndex);
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

void    Client::SendErrorPage(int errorNumber)
{
    std::stringstream ss;
    struct stat statbuffer;
    char buff[BUFFER_SIZE];
    std::string header;

    int efd = open(getServer().getErrorPages()[errorNumber].c_str(), O_RDONLY);
    if (efd < 0)
        throw(std::runtime_error("Invalid Error page !"));
    fstat(efd, &statbuffer);
    ss << statbuffer.st_size;
    if (errorNumber == MOVEDPERMANENTLY)
        header = response.getHttpVersion() + response.getStatusCode()[errorNumber] + "\r\nContent-Length: " + ss.str() + "\r\nLocation: " + response.getPath() + "/" + "\r\n\r\n";
    else
        header = response.getHttpVersion() + response.getStatusCode()[errorNumber] + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
    int bytes = write(GetSocketId(), header.c_str(), header.size());
    if (bytes < 0)
        throw(std::runtime_error(" write failed in sendErrorpage"));
    int rd = read(efd, buff, BUFFER_SIZE);
    buff[rd] = '\0';
    write(GetSocketId(), buff, BUFFER_SIZE);
    _readStatus = -1;
}

std::string getExtention(const std::string& filePath)
{
    size_t dotIndex;

    dotIndex = std::min(filePath.find('.'), filePath.size());
    return(filePath.substr(dotIndex));
}

void    Client::ft_send( void )
{
    char buff[BUFFER_SIZE];
    std::cout << "enter ft-send" << std::endl;
    std::cout << "target FD : " << _content_fd << std::endl;
    if (!isOpen(_content_fd))
    {
        std::cout << " the file fd is closed : " << _content_fd << std::endl;
        _readStatus = -1;
    }
    if (!isOpen(GetSocketId()))
    {
        std::cout << " the socket fd is closed : " << GetSocketId() << std::endl;
        _readStatus = -1;
    }
    if ((_readStatus = read(_content_fd, buff, BUFFER_SIZE)) >= 0)
    {
        std::cout << "++++ Read +++" << std::endl;
        if (write(GetSocketId(), buff, BUFFER_SIZE) < 0)
        {
            std::cout << "++++ Write +++" << std::endl;
            _readStatus = -1;
            // throw(0);
        }
        std::cout << "------- Read ----------" << std::endl;
    }
    std::cout << "exit ft-send" << std::endl;
}


/// @brief GET method
void    Client::Reply( void )
{
    char buff[BUFFER_SIZE];
    std::string header;
    std::stringstream ss;
    struct stat statbuffer;
    int readBytes = 0;
    int bodyFd;
    int fd;
    memset(buff, 0, BUFFER_SIZE);
    if (response.GetFileExtention() == ".php" || response.GetFileExtention() == ".py")
    {
        fullEnv();
        std::string outfile = GenerateFile();
        int pid  = fork();
        if (!pid)
        {
            std::map<std::string, std::string> intrepreter = getServer().getCgi();
            std::string filePath  = _targetPath.c_str();
            char *Path[3] = {(char*)intrepreter[response.GetFileExtention()].c_str(), (char *)filePath.c_str(), NULL};
            fd = open (outfile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
            if (fd < 0)
                throw(std::runtime_error("Open Failed"));
            dup2(fd, 1);
            close(fd);
            if (response.getMethod() == "POST")
            {
                bodyFd = response.getFd();
                dup2(bodyFd, 0);
                close(bodyFd);
            }
            execve(Path[0], Path, _env);// ! ENV
            deleteEnv();
            std::cout << "ERRRRORRR" << std::endl;
            exit(EXFIALE);
        }
        deleteEnv();
        waitpid(pid, 0, 0);
        fd = open (outfile.c_str(), O_CREAT | O_RDWR , 0777);
        if (fd < 0)
            throw(std::runtime_error("Open Failed  in GET-CGI "));
        if (response.GetFileExtention() == ".php")
            readBytes = read(fd, buff, HEADER_SIZE);
    }
    else
    {
        fd = open (_targetPath.c_str(), O_RDONLY);
        if (fd < 0)
            throw(std::runtime_error("Open Failed in GgI"));
    }
    fstat(fd, &statbuffer);
    ss << statbuffer.st_size - readBytes;
    header = response.getHttpVersion() + " 200 OK\r\nContent-Type: "
    + get_content_type() + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
    send(GetSocketId(), header.c_str(), header.size(), 0);
    _content_fd = fd;
    _status = 1;
}

bool    file_exists(const std::string &filename)
{
    struct stat buffer;

    return(stat(filename.c_str(), &buffer) == 0);
}

bool    isDirectory(const char* path) {
    struct stat fileInfo;

    if (stat(path, &fileInfo) != 0)
        throw(std::runtime_error("stat failed in isDirectory"));

    return S_ISDIR(fileInfo.st_mode);
}


void    Client::DirectoryHasIndexFile(const std::string& indexFile)
{
    response.setPath(response.getPath() + indexFile);
    if (file_exists(response.getPath())) // ! protect invalid index file
        Reply();
    else
        SendErrorPage(NOTFOUND); 
}

/// @brief if the request is a directory 
void    Client::handleDirectory(const std::string& filePath)
{
    if (filePath[filePath.size() - 1] != '/')
        SendErrorPage(MOVEDPERMANENTLY);
    if (response.getMethod() == "GET" && getServer().getLocations()[_locationIndex].getIndex().empty() && getServer().getIndex().empty())
    {
        if (getServer().getLocations()[_locationIndex].getAutoIndex())
        {
            std::stringstream ss;
            std::string test = GenerateDirectoryListing(filePath);
            ss << test.size();
            std::string header = response.getHttpVersion() + " 200 OK\r\nContent-Type: "
                + "text/html" + "\r\nContent-length: " + ss.str() + "\r\n\r\n";
            if (write(GetSocketId(), header.c_str(), header.size()) < 0)
                _readStatus = -1;
            if (write(GetSocketId(), test.c_str(), test.size()) < 0)
                _readStatus = -1;
        }
        else
            SendErrorPage(FORBIDDEN);
    }
    else if (!getServer().getLocations()[_locationIndex].getIndex().empty())
        DirectoryHasIndexFile(getServer().getLocations()[_locationIndex].getIndex());
    else if (!getServer().getIndex().empty())
        DirectoryHasIndexFile(getServer().getIndex());
    else
        SendErrorPage(FORBIDDEN);
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

void    Client::checkIndexFile(const std::string& indexFile, const std::string& targetPath)
{
    if (getExtention(indexFile) != ".php" && getExtention(indexFile) != ".py")
        Delete_dir(targetPath);
    else
        DirectoryHasIndexFile(indexFile);
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
void    Client::ft_delete( void )
{
    if (isDirectory(_targetPath.c_str()))
    {
        if (_targetPath[_targetPath.size() - 1] != '/')
            SendErrorPage(CONFLICT);
        else if(getServer().getLocations()[_locationIndex].getIndex().empty() && getServer().getIndex().empty())
            Delete_dir(_targetPath);
        else if (!getServer().getLocations()[_locationIndex].getIndex().empty())
            checkIndexFile(getServer().getLocations()[_locationIndex].getIndex(), _targetPath);
        else if (!getServer().getIndex().empty())
            checkIndexFile(getServer().getIndex(), _targetPath);
    }
    else
        std::remove(_targetPath.c_str());
}
void    Client::setTargetPath( void )
{
	_targetPath = getServer().getRoot() + getFileName(response.getPath()
	, getServer().getLocations()[_locationIndex].getpattern().size());
}

void    Client::ft_Response( void )
{
    try
    {
        signal(SIGPIPE, SIG_IGN);
        static int i = 0;
        std::cout << "********************START-RESPONSE  : " << "*******************" << std::endl;
        response.CreateStatusCode();
        initLocationIndex();
        setTargetPath();

        // std::cout << "Rs satatus : " << response.getResponseStatus() << std::endl;
        // if (response.getResponseStatus() != 0)
        //     SendErrorPage(response.getResponseStatus());
        initMethods(methods, getServer().getLocations()[_locationIndex].getLimit_except());
        if (access(_targetPath.c_str(), F_OK))
            SendErrorPage(NOTFOUND);
        else if (access(_targetPath.c_str(), R_OK))
            SendErrorPage(FORBIDDEN);
        else if (response.getMethod() == "GET")
        {
            std::cout << "------------------------------------- " << i++ << " ---------------------------------------------" << std::endl;
            // sleep(1);
            if (!methods._get)
                SendErrorPage(FORBIDDEN);
            if (isDirectory(_targetPath.c_str()))
                handleDirectory(_targetPath);
            else
                Reply();
        }
        else if (response.getMethod() == "DELETE")
        {
            if (!methods._delete)
                SendErrorPage(FORBIDDEN);
            ft_delete();
        }
        else if (response.getMethod() == "POST")
        {
            if (!methods._post)
                SendErrorPage(FORBIDDEN);
            if (getServer().getLocations()[_locationIndex].getUpload().empty())
                throw(std::runtime_error("empty upload path"));
            if (isDirectory(_targetPath.c_str()))
                handleDirectory(_targetPath);
            else
                Reply();
            // SendErrorPage(CREATED);
        }
        else 
            _readStatus = -1;
    }
    catch(std::exception &e)
    {
        _readStatus = -1;
        std::cout << e.what() << std::endl;
    } 
    catch(const int e)
    {
        std::cout << e << std::endl;
    }
    std::cout << "********************END-RESPONSE*******************" << std::endl;
}

bool isOpen(int fd)
{
    struct stat buff;
    if (fstat(fd, &buff) == -1)
    {
        std::cout << "invalid Fd " << std::endl;
        return (false);
    }
    std::cout << "valid Fd " << std::endl;;
    return(true);
}

