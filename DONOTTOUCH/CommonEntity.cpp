/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonEntity.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:25:46 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 14:48:02 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/CommonEntity.hpp"

CommonEntity::CommonEntity() : _client_max_body_size(0), _AutoIndex(false), _root_exists(false) {}

CommonEntity::CommonEntity(TokenVectsIter begin, TokenVectsIter end)
    : _client_max_body_size(0), _AutoIndex(false), _root_exists(false)
{
    // Initialize the object using the provided iterators (assuming they point to strings).
    // You would need to implement this constructor based on your specific needs.
}

CommonEntity::CommonEntity(const CommonEntity& other)
    : _root(other._root), _index(other._index),
      _error_pages(other._error_pages),
      _client_max_body_size(other._client_max_body_size),
      _AutoIndex(other._AutoIndex),
      _root_exists(other._root_exists) {}

CommonEntity& CommonEntity::operator=(const CommonEntity& other)
{
    if (this != &other)
    {
        _root = other._root;
        _index = other._index;
        _error_pages = other._error_pages;
        _client_max_body_size = other._client_max_body_size;
        _AutoIndex = other._AutoIndex;
        _root_exists = other._root_exists;
    }
    return *this;
}

void CommonEntity::InitRoot(std::string value)
{
    _root = value;
    _root_exists = true;
}
void CommonEntity::InitIndex(std::string value)
{
    _index.push_back(value);
}

void CommonEntity::InitErrorPage(std::string value)
{
    // Implement this method to initialize error pages.
    // You would need to parse and store error pages based on your needs.
    int error_code;
    std::string error_page_path;

    // Parse the input string to extract error code and error page path
    std::istringstream iss(value);
    if (iss >> error_code >> error_page_path)
    {
        // Store the parsed values in the _error_pages map
        _error_pages[error_code] = error_page_path;
    }
    else
    {
        // Handle parsing error if needed
        std::cerr << "Error parsing error page: " << value << std::endl;
    }
}

void CommonEntity::InitClienBodySize(std::string value)
{
    // Implement this method to initialize client max body size.
    // You would need to parse and store the size based on your needs.

    // Parse the input string as an integer
    std::istringstream iss(value);
    size_t client_max_body_size;
    if (iss >> client_max_body_size)
    {
        // Store the parsed value in the _client_max_body_size member
        _client_max_body_size = client_max_body_size;
    }
    else
    {
        // Handle parsing error if needed
        std::cerr << "Error parsing client max body size: " << value << std::endl;
    }
}

void CommonEntity::InitAutoIndex(std::string value)
{
    // Implement this method to initialize the AutoIndex flag.
    // You would need to parse and store the flag based on your needs.

    // Convert the input string to lowercase for case-insensitive comparison
    std::transform(value.begin(), value.end(), value.begin(), std::tolower);

    // Check if the value is "true" or "false"
    if (value == "true")
        _AutoIndex = true;
    else if (value == "false")
        _AutoIndex = false;
    else
    {
        // Handle parsing error if needed
        std::cerr << "Error parsing AutoIndex: " << value << std::endl;
    }
}

std::string CommonEntity::getRoot() const
{
    return _root;
}

std::vector<std::string> CommonEntity::getIndex() const
{
    return _index;
}

std::map<int, std::string> CommonEntity::getErrorPages() const
{
    return _error_pages;
}

size_t CommonEntity::getClientMaxBodySize() const
{
    return _client_max_body_size;
}

bool CommonEntity::getAutoIndex() const
{
    return _AutoIndex;
}
