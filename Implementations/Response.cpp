/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:36:51 by sben-ela          #+#    #+#             */
/*   Updated: 2023/10/23 17:45:48 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Webserv.hpp"
#include "../Includes/Response.hpp"
#include "../Includes/Configuration.hpp"
#include "../Includes/Client.hpp"

Response::Response() {}

Response::Response(const Response& other) : Request(other) {}

Response& Response::operator=(const Response& other)
{
    if (this != &other)
        Request::operator=(other);
    return *this;
}

Response::~Response() {}

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

std::string Client::GenerateDirectoryListing(const std::string& directoryPath) {
    std::string html;
    html += "<html><head><title>Directory Listing</title></head><body>";
    html += "<h1>Directory Listing</h1>";
    html += "<table border='1'><tr><th>Name</th><th>Size</th><th>Date Modified</th></tr>";
    
    DIR* dir = opendir(directoryPath.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
                std::string filePath = directoryPath + "/" + entry->d_name;
                struct stat fileStat;
                if (stat(filePath.c_str(), &fileStat) == 0) {
                    std::string fileSize;
                    if (S_ISDIR(fileStat.st_mode)) {
                        fileSize = "Directory";
                    } else {
                        std::stringstream ss;
                        ss << fileStat.st_size;
                        fileSize = ss.str() + " bytes";
                    }

                    char dateModified[100];
                    strftime(dateModified, sizeof(dateModified), "%D, %r", localtime(&fileStat.st_mtime));

                    html += "<tr><td><a href='" + std::string(response.getPath() + entry->d_name) + "'>" + entry->d_name + "</a></td><td>" + fileSize + "</td><td>" + dateModified + "</td></tr>";
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
    return(fileName);
}


void    Client::SendErrorPage(int errorNumber)
{
    std::stringstream ss;
    struct stat statbuffer;
    char buff[BUFFER_SIZE];
    std::string header;
    std::string error_page = getServer().getErrorPages()[errorNumber];
    _content_fd = open(error_page.c_str(), O_RDONLY);
    if (_content_fd < 0)
    {
        error_page = _defaultErrorPages[errorNumber];
        _content_fd = open(error_page.c_str(), O_RDONLY);
    }
    fstat(_content_fd, &statbuffer);
    ss << statbuffer.st_size;
    if (errorNumber == MOVEDPERMANENTLY)
        header = response.getHttpVersion() + response.getStatusCode()[errorNumber] + "\r\n" + get_content_type(error_page)
        + "\r\nLocation: " + response.getPath() + "/" + "\r\n\r\n";
    else
		header = response.getHttpVersion() + response.getStatusCode()[errorNumber] + "\r\n" + get_content_type(error_page)
        + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
    if (write(GetSocketId(), header.c_str(), header.size()) <= 0)
        _responseStatus = -1;
    _status = 1;
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
    if ((_responseStatus = read(_content_fd, buff, BUFFER_SIZE)) > 0)
    {
        if (write(GetSocketId(), buff, _responseStatus) <= 0)
            _responseStatus = -1;
    }
}

void    Client::SendHeader(int fd)
{
    std::string header;
    struct stat statbuffer;
    std::stringstream ss;

    fstat(fd, &statbuffer);
    ss << statbuffer.st_size - _CgiHeader.size();
    if (_status == CGI)
        header = response.getHttpVersion() + response.getStatusCode()[response.getResponseStatus()] + "\r\n" + get_content_type(_targetPath) + "\r\nContent-Length: " + ss.str() + (getCookie() != "\r\n" ? getCookie() : "") + "\r\n\r\n";
    else
        header = response.getHttpVersion() + response.getStatusCode()[response.getResponseStatus()] + "\r\n" + get_content_type(_targetPath) + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
    if (write(GetSocketId(), header.c_str(), header.size()) <= 0)
        _responseStatus = -1;
}

void    Client::Reply( void )
{
    if (response.GetFileExtention() == ".php" || response.GetFileExtention() == ".py")
    {
        _CgiFile = response.GenerateFile("/Users/sben-ela/goinfre/");
        _cgiTimer = std::time(NULL);
        _cgiPid = fork();
        if (!_cgiPid)
        {
            fullEnv();
            std::map<std::string, std::string> intrepreter = getServer().getCgi();
            char *Path[3] = {(char*)intrepreter[response.GetFileExtention()].c_str(), (char *)_targetPath.c_str(), NULL};
            _content_fd = open (_CgiFile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
            if (_content_fd < 0)
                throw(std::runtime_error("Open Failed in child to open : " + _CgiFile));
            dup2(_content_fd, STDOUT_FILENO);
            ft_close(_content_fd);
            ft_close(STDERR_FILENO);
            if (response.getMethod() == "POST")
            {
                int bodyFd = response.getFd();
                dup2(bodyFd, STDIN_FILENO);
                ft_close(bodyFd);
            }
            
            execve(Path[0], Path, _env);
            deleteEnv();
            std::cout << "ERRRRORRR" << std::endl;
            exit(EXFIALE);
        }
        _status = CGI;
        return ;
    }
    else
    {
        _content_fd = open (_targetPath.c_str(), O_RDONLY);
        if (_content_fd < 0)
            throw(std::runtime_error("Open Failed in Reply"));
    }
    SendHeader(_content_fd);
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
    _targetPath += indexFile;
    if (file_exists(_targetPath) && !isDirectory(_targetPath.c_str()))
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
            std::string DirectoryListing = GenerateDirectoryListing(filePath);
            ss << DirectoryListing.size();
            std::string header = response.getHttpVersion() + response.getStatusCode()[OK] + "\r\nContent-Type: " + "text/html" + "\r\nContent-Length: " + ss.str() + "\r\n\r\n";
            if (write(GetSocketId(), (header + DirectoryListing).c_str() , header.size() + DirectoryListing.size()) < 0)
                throw(std::runtime_error("write Failed"));
            _responseStatus = -1;
        }
        else
            SendErrorPage(FORBIDDEN);
    }
    else if (!getServer().getLocations()[_locationIndex].getIndex().empty())
        DirectoryHasIndexFile(getServer().getLocations()[_locationIndex].getIndex());
    else if (!getServer().getIndex().empty())
        DirectoryHasIndexFile(getServer().getIndex());
    else if (response.getMethod() == "GET")
        SendErrorPage(FORBIDDEN);
    else
        SendErrorPage(CREATED);
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

void    Client::readCgiHeader( int fd )
{
    ssize_t     rd;
    size_t      pos;
    char        buff[BUFFER_SIZE];

    rd = read(fd, buff, BUFFER_SIZE - 1);
    if (rd < 0)
    {
        throw(std::runtime_error("read Failed"));
        return ;
    }
    buff[rd] = 0;
    _CgiHeader = buff;
    pos = _CgiHeader.find("\r\n\r\n");
    if (pos == std::string::npos)
    {
        lseek(fd, 0, SEEK_SET);
        _CgiHeader.clear();
    }
    else
    {
        lseek(fd, -1 * (rd - (pos + 4)), SEEK_CUR);
        _CgiHeader = _CgiHeader.substr(0, pos + 4);
    }
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
        Delete_dir(_targetPath);
    else
        std::remove(_targetPath.c_str());
}

void    Client::setTargetPath( void )
{
    std::string root = getServer().getLocations()[_locationIndex].getRoot();
    if (root.empty())
        root = getServer().getRoot();
	_targetPath =  root + getFileName(response.getPath()
	, getServer().getLocations()[_locationIndex].getpattern().size());
}

void    Client::ft_Response( void )
{
    try
    {
        _content_fd = -1;
        signal(SIGPIPE, SIG_IGN);
        response.CreateStatusCode();
        initLocationIndex();
        setTargetPath();
        if (response.getResponseStatus() != OK)
        {
            SendErrorPage(response.getResponseStatus());
            return ;
        }
        initMethods(methods, getServer().getLocations()[_locationIndex].getLimit_except());
        if (access(_targetPath.c_str(), F_OK))
            SendErrorPage(NOTFOUND);
        else if (access(_targetPath.c_str(), R_OK))
            SendErrorPage(FORBIDDEN);
        else if (response.getMethod() == "GET")
        {
            if (!methods._get)
                SendErrorPage(FORBIDDEN);
            else if (isDirectory(_targetPath.c_str()))
                handleDirectory(_targetPath);
            else
                Reply();
        }
        else if (response.getMethod() == "DELETE")
        {
            if (!methods._delete)
                SendErrorPage(FORBIDDEN);
            else
            {
                ft_delete();
                SendErrorPage(NOCONTENT);
            }
        }
        else if (response.getMethod() == "POST")
        {
            if (!methods._post)
            {
                std::remove(response._name.c_str());
                SendErrorPage(FORBIDDEN);
            }
            else if (isDirectory(_targetPath.c_str()))
                handleDirectory(_targetPath);
            else
                Reply();
        }
        else
            SendErrorPage(NOTIMPLEMENTED);
    }
    catch(std::exception &e)
    {
        _responseStatus = -1;
        // std::cout << e.what() << std::endl;
    } 
    catch(std::string &e)
    {
        _responseStatus = -1;
        // std::cout << e << std::endl;
    }
    catch(...)
    {
        _responseStatus = -1;
    }
}

bool isOpen(int fd)
{
    struct stat buff;
    if (fstat(fd, &buff) == -1)
        return (false);
    return(true);
}

void ft_close(int fd)
{
    if (fd == -1)
        return ;
    close (fd);
}