/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:22:39 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/14 17:25:17 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"

typedef std::vector<std::string>::iterator TokenVectsIter;


class Configuration
{
    public:
        typedef void (Configuration::*methods)(std::string);
    private:
        std::string                                      _root;
        std::string                                      _host;
        std::string                                      _index;
        std::map<int, std::string>                       _error_pages;
        size_t                                           _client_max_body_size;
        bool                                             _AutoIndex;
        bool                                             _root_exists;
        std::string                                      _port;
        bool                                             _host_exists;
        bool                                             _port_exists;
        std::string                                      _upload;
        std::string                                      _server_name;                                   
        std::vector<Location>                            _locations;
    public:
        Configuration();
        // Configuration operator()(std::string host, std::string  port);
        Configuration(std::vector<std::string> vecteur);
        std::vector<std::string>    Tokenization(std::string line);
        Configuration(const Configuration& other);
        Configuration& operator=(const Configuration& other);
        bool isStringAllDigits(const std::string& str);
        void InitHost(std::string value);
        void InitPort(std::string value);
        void InitServerName(std::string value);
        void                         InitRoot(std::string value);
        void                         InitIndex(std::string value);
        void                         InitErrorPage(std::string code, std::string path);
        void                         InitClientBodySize(std::string value);
        void                         InitAutoIndex(std::string value);
        void                         InitUpload(std::string value);
        std::string                  getUpload() const;
        std::string                  getRoot() const;
        std::string                  getIndex() const;
        std::map<int, std::string>   getErrorPages() const;
        size_t                       getClientMaxBodySize() const;
        bool                         getAutoIndex() const;
        // static    void check_dup(std::string host, std::string port);
        std::string                 getHost() const;
        std::string                 getPort() const;
        std::string                 getServerNames() const;
        std::vector<Location>       getLocations() const;
        friend std::ostream& operator<<(std::ostream& o, Configuration obj);
        ~Configuration();
};

/*
The friend keyword is used to declare a function or class as a friend of the class in which it is declared. A friend function is not a member of the class but is allowed to access its private and protected members.
*/