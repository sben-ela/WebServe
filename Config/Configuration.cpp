/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:26:09 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/14 17:23:40 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Configuration.hpp"

Configuration::Configuration()
        : _client_max_body_size(0),  _AutoIndex(false), _root_exists(false),
        _host_exists(false), _port_exists(false) {}

std::vector<std::string>    Configuration::Tokenization(std::string line)
{
    std::vector<std::string> result;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
        result.push_back(token);
    }
    return result;
}

bool Configuration::isStringAllDigits(const std::string& str) {
    // Find the first character that is not a digit
    size_t pos = str.find_first_not_of("0123456789");

    // If pos is equal to std::string::npos, it means there are no non-digit characters
    return (pos == std::string::npos);
}

Configuration::Configuration(std::vector<std::string> vecteur)
    : _client_max_body_size(0),  _AutoIndex(false), _root_exists(false),
    _host_exists(false), _port_exists(false)
{
    TokenVectsIter begin = vecteur.begin();
    TokenVectsIter end = vecteur.end();
    while (begin != end)
    {
        std::string line = *begin;
        std::vector<std::string> token = Tokenization(line);
        if (token[0] == "host" && token.size() == 2)
        {
            ++begin;
            InitHost(token[1]);
        }
        else if (token[0] == "autoindex")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitAutoIndex(token[1]);
            else
                throw std::string("Invalid autoindex");
        }
        else if (token[0] == "index")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitIndex(token[1]);
            else
                throw std::string("Invalid Index");
        }
        else if (token[0] == "client_body_size")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitClientBodySize(token[1]);
            else
                throw std::string("Invalid Client body size");
        }
        else if (token[0] == "root")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitRoot(token[1]);
            else
                throw std::string("Invalid root");
        }
        else if (token[0] == "listen")
        {
            ++begin;
            if (isStringAllDigits(token[1]) && token.size() == 2 && atoi(token[1].c_str()) <= 65635)
                InitPort(token[1]);
            else
                throw std::string("Invalid port number");
        }
        else if (token[0] == "server_name")
        {
            ++begin;
            if (token.size() == 2)
                InitServerName(token[1]);
            else
                throw std::string("Invalid server name"); //! error
        }
        else if (token[0] == "upload_path")
        {
            // Extract and set upload
            ++begin;
            if (token.size() == 2 && begin != end)
                InitUpload(token[1]);
            else
                throw std::string("Invalid Upload path");
        }
        else if (token[0] == "error_page")
        {
            ++begin;
            if (token.size() == 3)
                InitErrorPage(token[1], token[2]);
            else
                throw std::string("Invalid error page arguments"); // ! error
        }
        else if (token[0] == "location")
        {
            ++begin;
            if (begin != end && token.size() == 2)
            {
                // Find the closing curly brace of the location block.
                TokenVectsIter endIt = std::find(begin, end, "}");
                // Create a Location object and add it to the vector.
                Location location(token[1], begin, endIt);
                _locations.push_back(location);
                // Move the iterator to the next position after the location block.
                begin = endIt;
            }
            else
                throw std::string("Invalid location !"); //!error
        }
        else
            ++begin;
    }
}

Configuration::Configuration(const Configuration& other)
    : _root(other._root), _host(other._host), _index(other._index),
      _error_pages(other._error_pages), _client_max_body_size(other._client_max_body_size),
      _AutoIndex(other._AutoIndex), _root_exists(other._root_exists), _port(other._port),
      _host_exists(other._host_exists), _port_exists(other._port_exists),
      _server_name(other._server_name), _locations(other._locations) {}

Configuration& Configuration::operator=(const Configuration& other)
{
    if (this != &other)
    {
        _host = other._host;
        _port = other._port;
        _host_exists = other._host_exists;
        _port_exists = other._port_exists;
        _server_name = other._server_name;
        _locations = other._locations;
        _root = other._root;
        _index = other._index;
        _error_pages = other._error_pages;
        _client_max_body_size = other._client_max_body_size;
        _AutoIndex = other._AutoIndex;
        _root_exists = other._root_exists;
    }
    return *this;
}

void Configuration::InitHost(std::string value)
{
    _host = value;
    _host_exists = true;
}

void Configuration::InitPort(std::string value)
{
    _port = value;
    _port_exists = true;
}

void Configuration::InitServerName(std::string value)
{
    _server_name = value;
}

void Configuration::InitRoot(std::string value)
{
    _root = value;
    _root_exists = true;
}

void Configuration::InitIndex(std::string value)
{
    _index = value;
}

void Configuration::InitErrorPage(std::string code, std::string path)
{
    // Implement this method to initialize error pages.
    // You would need to parse and store error pages based on your needs.
    int error_code = atoi(code.c_str());
    std::string error_page_path = path;
    if ((error_code >= 100 && error_code <= 599) && !path.empty())
    {
        // Store the parsed values in the _error_pages map
        _error_pages[error_code] = error_page_path;
    }
    else
    {
        // Handle parsing error if needed
        std::string str = "Error parsing error page: " + code + " ";
        throw std::string(str.append(path));
    }
}

void Configuration::InitClientBodySize(std::string value)
{
    // Use a stringstream to parse the input string as an integer
    std::istringstream iss(value);
    int client_max_body_size = 0;

    // Attempt to extract an integer from the string
    if (!(iss >> client_max_body_size))
    {
        // Handle parsing error if the extraction fails
        std::string error_msg = "Error parsing client max body size: " + value;
        throw std::string(error_msg);
    }

    // Check if the parsed value is valid (greater than zero)
    if (client_max_body_size < 0)
    {
        std::string error_msg = "Invalid client max body size: " + value;
        throw std::string(error_msg);
    }

    // Store the parsed value in the _client_max_body_size member
    _client_max_body_size = static_cast<size_t>(client_max_body_size);
}

void Configuration::InitAutoIndex(std::string value)
{
    // Implement this method to initialize the AutoIndex flag.
    // You would need to parse and store the flag based on your needs.

    // Convert the input string to lowercase for case-insensitive comparison
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    // Check if the value is "true" or "false"
    if (value == "on")
        _AutoIndex = true;
    else if (value == "off")
        _AutoIndex = false;
    else
    {
        // Handle parsing error if needed
        std::string error_msg = "Error parsing AutoIndex: " + value;
        throw std::string(error_msg);
    }
}

void Configuration::InitUpload(std::string value)
{
    _upload = value;
}

std::string Configuration::getUpload() const
{
    return _upload;
}

std::string Configuration::getRoot() const
{
    return _root;
}

std::string Configuration::getIndex() const
{
    return _index;
}

std::map<int, std::string> Configuration::getErrorPages() const
{
    return _error_pages;
}

size_t Configuration::getClientMaxBodySize() const
{
    return _client_max_body_size;
}

bool Configuration::getAutoIndex() const
{
    return _AutoIndex;
}

std::string Configuration::getHost() const
{
    return _host;
}

std::string Configuration::getPort() const
{
    return _port;
}

std::string Configuration::getServerNames() const
{
    return _server_name;
}

std::vector<Location> Configuration::getLocations() const
{
    return _locations;
}

// friend std::ostream& operator<<(std::ostream& o, const Configuration obj);

std::ostream& operator<<(std::ostream& o, Configuration obj)
{
    o << "Host: " << obj.getHost() << std::endl;
    o << "Port: " << obj.getPort() << std::endl;
    o << "Server Name: " << obj.getServerNames() << std::endl;
    
    // Output location blocks
    std::vector<Location> locations = obj.getLocations();
    for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        std::cout << *it << std::endl;
    }
    return o;
}
// This code defines the Configuration class with member functions for initialization and access to its attributes. It also handles parsing location blocks and uses the Location class to store and manage them.

Configuration::~Configuration() {}
