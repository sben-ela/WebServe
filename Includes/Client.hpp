/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:08 by aybiouss          #+#    #+#             */
/*   Updated: 2023/10/05 17:47:37 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "Configuration.hpp"
#include "Response.hpp"

struct   Methods
{
    bool _get;
    bool _post;
    bool _delete;
};


class Client
{
    private:
        int         _socketId;
        size_t      _locationIndex;
        Configuration   _client_server;
        std::string     _targetPath;
        char        **_env;
        std::map<std::string, std::string> _mapEnv;
    public :
        int         _content_fd;
        int         _status;
        ssize_t     _readStatus;
        Methods     methods;
        Response    response;
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        const int&          GetSocketId( void ) const;
        const Configuration&      getServer() const;
        void    set_server(Configuration p);
        void    set_socket(int socket);
        // ! NEW
        const char* get_content_type( void );
        void    fullMapEnv();
        void    fullEnv();
        void    deleteEnv();
        void    setTargetPath( void );
        size_t  getLocationIndex( void );
        void    initLocationIndex( void );
        void    SendErrorPage(int errorNumber);
        void    ft_delete( void );
        void    handleDirectory(const std::string& filePath);
        void    DirectoryHasIndexFile(const std::string& indexFile);
        void    checkIndexFile(const std::string& indexFile, const std::string& targetPath);
        void    Reply( void );
        void    ft_send( void );
        void    ft_Response( void );
        // !
        ~Client();
};
