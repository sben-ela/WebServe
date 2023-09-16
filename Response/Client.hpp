#pragma once


#include "../Includes/Request.hpp"

#define BUFFER_SIZE 1024

class Client
{
    private : 
        std::string _method;
        std::string _path;
        std::string _contentType;
        std::string _httpVersion;
        int         _socketId;
    public : 
        Client(std::string method, std::string path, std::string contentType, std::string httpVersion, int socketId);
        const std::string&  GetMethod( void ) const;
        const std::string&  GetPath( void ) const;
        const std::string&  GetConetType( void ) const;
        const std::string&  GetHttpVersion( void ) const;
        const int&          GetSocketId( void ) const;

};