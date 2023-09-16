#include "Client.hpp"

Client::Client(std::string method, std::string path, std::string contentType, std::string httpVersion, int socketId)
{
    _method = method;
    _path = path;
    _contentType = contentType;
    _httpVersion = httpVersion;
    _socketId = socketId;
}

const std::string& Client::GetMethod( void ) const
{
    return (_method);
}

const std::string& Client::GetPath( void ) const
{
    return (_path);
}

const std::string& Client::GetConetType( void ) const
{
    return (_contentType);
}

const std::string& Client::GetHttpVersion( void ) const
{
    return (_httpVersion);
}

const int& Client::GetSocketId( void ) const
{
    return (_socketId);
}
