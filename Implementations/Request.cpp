/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/30 16:29:55 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../Includes/Request.hpp"

Request::Request() : _headers_done(false) {}

const int&  Request::getFd() const
{
    return _fd;
}

// Function to parse an HTTP request
int Request::parseHttpRequest(std::string requestBuffer, int new_socket, size_t bytesread) {
    (void)new_socket;
    int i = processChunk(requestBuffer, bytesread);
    // if (!i)
    //     return parseRequest();
    return i;
}

size_t Request::customFind(const std::vector<char>& vec, const std::string& str, size_t start)
{
    size_t vectorSize = vec.size();
    size_t strLength = str.length();
    if (strLength == 0 || start >= vectorSize) {
        return std::string::npos;  // Return 'not found' if the input is invalid.
    }
    for (size_t i = start; i < vec.size(); ++i) {
        if (vec[i] == str[0])
        { // If the first character matches
            bool found = true;
            for (size_t j = 0; j < str.length(); ++j) {
                if (i + j >= vec.size() || vec[i + j] != str[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i; // Found at position i
        }
    }
    return std::string::npos; // Not found
}

std::vector<char> Request::customSubstr(const std::vector<char>& vec, size_t start, size_t length)
{
    std::vector<char> result;

    if (start >= vec.size()) {
        return result; // Return an empty vector if the start position is out of bounds
    }
    for (size_t i = start; i < start + length && i < vec.size(); ++i) {
        result.push_back(vec[i]);
    }
    return result;
}

int Request::countNumbersInString(const std::string& inputString) {
    int count = 0;
    for (size_t i = 0; i < inputString.length(); ++i) {
        if (std::isdigit(inputString[i])) {
            count++;
        }
    }
    return count;
}

int		Request::processChunk(std::string buffer, size_t bytesread)
{
    (void)bytesread;
    if (!_headers_done)
    {
        _all.append(buffer);
        size_t done = _all.find("\r\n\r\n");
        if (done != std::string::npos)
        {
            _header_before = _all.substr(0, done);
            _bodies = _all.substr(done + 4);
            _headers_done = true;
            int j = parseHeaders();
            if (!j)
            {
                _bodies.clear();
                return 0;
            }
        }
    }
    else
	{
        _bodies.append(buffer);
        return processBody();
    }
    return 1;
}

std::string Request::GenerateFile() {
    std::string Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefh12326544";
    std::string file;
    
    // Create the directory if it doesn't exist
    const char* dir_path = "/nfs/homes/sben-ela/save/POSTs/";
    if (mkdir(dir_path, 0777) != 0 && errno != EEXIST) {
        std::cerr << "Failed to create directory: " << strerror(errno) << std::endl;
        return "";  // Return an empty string to indicate failure
    }
    
    for (size_t i = 0; i < FILESIZE; i++) {
        file += Base[rand() % Base.size()];
    }
    _ofile = dir_path + file;
    return _ofile;
}

/// ! NEW
std::map<int, std::string> Request::getStatusCode( void ) const
{
    return(_statusCode);
}

void Request::CreateStatusCode()
{
    _statusCode[OK] = " 200 OK";
    _statusCode[CREATED] = " 201 Created";
    _statusCode[MOVEDPERMANENTLY] = " 301 Moved Permanently";
    _statusCode[NOTALLOWED] = " 405 Method Not Allowed";
    _statusCode[CONTENTTOLARGE] = " 413 Request Entity Too Large";
    _statusCode[URLTOOLONGE] = " 414 Request-URI Too Long";
    _statusCode[MOVEDTEMPORARILY] = " 302 Moved Temporarily";
    _statusCode[USEPROXY] = " 305 Use Proxy";
    _statusCode[MULTIPLECHOICES] = " 300 Multiple Choices";
    _statusCode[NOCONTENT] = " 204 No Content";
    _statusCode[RESETCONTENT] = " 205 Reset Content";
    _statusCode[PARTIALCONTENT] = " 206 Partial Content";
    _statusCode[SEEOTHER] = " 303 See Other";
    _statusCode[NOTMODIFIED] = " 304 Not Modified";
    _statusCode[TEMPORARYREDIRECT] = " 307 Temporary Redirect";
    _statusCode[BADREQUEST] = " 400 Bad Request";
    _statusCode[UNAUTHORIZED] = " 401 Unauthorized";
    _statusCode[PAYMENTREQUIRED] = "402 Payment Required";
    _statusCode[FORBIDDEN] = " 403 Forbidden";
    _statusCode[NOTFOUND] = " 404 Not Found";
    _statusCode[NOTACCEPTABLE] = " 406 Not Acceptable";
    _statusCode[PROXYAUTHENTICATIONREQUIRED] = " 407 Proxy Authentication Required";
    _statusCode[REQUESTTIMEOUT] = " 408 Request Time-out";
    _statusCode[CONFLICT] = " 409 Conflict";
    _statusCode[GONE] = " 410 Gone";
    _statusCode[LENGTHREQUIRED] = " 411 Length Required";
    _statusCode[PRECONDITIONFAILED] = " 412 Precondition Failed";
    _statusCode[UNSUPPORTEDMEDIATYPE] = " 415 Unsupported Media Type";
    _statusCode[REQUESTEDRANGENOTSATISFIABLE] = " 416 Requested range not satisfiable";
    _statusCode[EXPECTATIONFAILED] = " 417 Expectation Failed";
    _statusCode[INTERNALSERVERERROR] = " 500 Internal Server Error";
    _statusCode[NOTIMPLEMENTED] = " 501 Not Implemented";
    _statusCode[BADGATEWAY] = " 502 Bad Gateway";
    _statusCode[SERVICEUNAVAILABLE] = " 503 Service Unavailable";
    _statusCode[GATEWAYTIMEOUT] = " 504 Gateway Time-out";
    _statusCode[HTTPVERSIONNOTSUPPORTED] = " 505 HTTP Version not supported";
}

int    Request::processBody()
{
    size_t crlf_pos = _bodies.find("\r\n");
    if (crlf_pos != std::string::npos) {
        std::string chunk_size_str = _bodies.substr(0, crlf_pos);
        char* endptr;
        int chunksize = strtoul(chunk_size_str.c_str(), &endptr, 16);
        if (!chunksize)
        {
            std::cout << "Body ended ..." << std::endl;
            close(_fd);
            return 0;
        }
        if (_bodies.length() >= chunksize + 2 + crlf_pos + 2)
        {
            std::string str = _bodies.substr(crlf_pos + 2, chunksize);
            write(_fd, str.c_str(), chunksize);
            _bodies = _bodies.substr(chunksize + 2 + crlf_pos + 2);
        }
    }
    return 1;
}

int    Request::parseHeaders()
{
    // std::string header = vectorCharToString(_header_before);
    // std::cout << _header_before << std::endl;
    // exit(0);
    std::istringstream requestStream(_header_before);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus("400 Bad Request");
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus("400 Bad Request");
        return 0;
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    // if (_path == "/favicon.ico") {
    //     // Handle it as needed (status), or simply return an empty request
    //     return 0;
    // }
    std::string forBody;
    // Read and parse headers
    while (std::getline(requestStream, line) && !line.empty()) {
        forBody += line + "\n";
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            _headers[headerName] = headerValue;
        }
    }
    if (_method == "GET")
    {
        close(_fd);
        return 0;
    }
    else
    {
        _fd = open(GenerateFile().c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
        if (_fd == -1) {
            std::cerr << "Failed to open the file." << std::endl;
            return 0;
        }
    }
    return 1;
}

std::string Request::vectorCharToString(const std::vector<char>& vec)
{
    std::string result;
    result.reserve(vec.size()); // Reserve space for efficiency
    for (size_t i = 0; i < vec.size(); ++i) {
        result.push_back(vec[i]);
    }
    return result;
}

Request::Request(const Request& other)
    :   _method(other._method),
        _path(other._path),
        _httpVersion(other._httpVersion),
        _responseStatus(other._responseStatus),
        _header_before(other._header_before),
        _body(other._body),
        _headers(other._headers),
        _statusCode(other._statusCode),
        _contentType(other._contentType),
        _bodyFile(other._bodyFile),
        _ofile(other._ofile),
        _chunksize(other._chunksize),
        _bodies(other._bodies),
        _all(other._all),
        _headers_done(other._headers_done),
        _fd(other._fd) {}

Request& Request::operator=(const Request& other)
{
    if (this != &other)
    {
        _method = other._method;
        _path = other._path;
        _httpVersion = other._httpVersion;
        _responseStatus = other._responseStatus;
        _header_before = other._header_before;
        _body = other._body;
        _headers = other._headers;
        _statusCode = other._statusCode;
        _contentType = other._contentType;
        _bodyFile = other._bodyFile;
        _ofile = other._ofile;
        _chunksize = other._chunksize;
        _bodies = other._bodies;
        _all = other._all;
        _headers_done = other._headers_done;
        _fd = other._fd;
    }
    return *this;
}

std::string         Request::GetFileExtention( void ) const
{
    std::string extention;
    size_t index;

    index = _path.find('.');
    if (index == std::string::npos)
        return(extention);
    extention = _path.substr(index);
    return (extention);
}

std::string         Request::getContentType( void ) const
{
    std::string type;

    type = _path.substr(_path.find('.'));
    return (type);
}
void                Request::setPath(std::string newPath)
{
    _path = newPath;
}
/*int Request::parseRequest()
{
    // std::cout << "----------------+++++++++++++++++++++++++++" << std::endl;
    // std::cout << _header_before << std::endl;
    // std::cout << "----------------+++++++++++++++++++++++++++" << std::endl;
    // return 1;
    std::istringstream requestStream(_header_before);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus("400 Bad Request");
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus("400 Bad Request");
        return 0;
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return 0;
    }
    std::string forBody;
    bool isContentLengthFound = false;
    std::size_t contentLength = 0;
    // Read and parse headers
    while (std::getline(requestStream, line) && !line.empty()) {
        forBody += line + "\n";
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            _headers[headerName] = headerValue;
            if (_method == "POST" && headerName == "Content-Length")
            {
                try {
                    char* endptr;
                    const char* headerValueCStr = headerValue.c_str();
                    unsigned long parsedContentLength = strtoul(headerValueCStr, &endptr, 10);

                    if (parsedContentLength == ULONG_MAX) { endptr == headerValueCStr || *endptr != '\0' ||
                        // Handle invalid Content-Length value
                        setResponseStatus("400 Bad Request");
                        return 0;
                    }
                    contentLength = parsedContentLength;
                    isContentLengthFound = true;
                } catch (const std::exception& e) {
                    // Handle invalid Content-Length value
                    setResponseStatus("400 Bad Request");
                    return 0;
                }
            }
        }
    }
    // std::cout << "*******************" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }
    // std::cout << std::endl;
    // std::cout << "*******************" << std::endl;
    // (void)new_socket;
    if (_method == "POST") {
        if (!isContentLengthFound) {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return 0;
        }
        std::size_t bodyPos = forBody.find("\r\n\r\n");
        if (bodyPos != std::string::npos) {
            // Extract the body
            std::string requestBody = forBody.substr(bodyPos + 4);
            // Write the body to a file
            _bodyFile = "BodyOfRequest.txt";
            std::ofstream outfile(_bodyFile, std::ofstream::binary);
            outfile.write(requestBody.c_str(), contentLength);
            outfile.close();
        }
        else {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return 0;
        }
    }
    setResponseStatus("200 OK");
    return (0);
}*/

const std::string& Request::getPath() const
{
    return this->_path;
}

const std::string& Request::getMethod() const
{
    return this->_method;
}

const std::string& Request::getHttpVersion() const
{
    return this->_httpVersion;
}

const std::string& Request::getResponseStatus() const
{
    return this->_responseStatus;
}

void Request::setResponseStatus(const std::string& status) {
    _responseStatus = status;
}

Request::~Request() {}

// #include "../Includes/Request.hpp"

// Request::Request() : _headers_done(false) {}

// const int&  Request::getFd() const
// {
//     return _fd;
// }

// // Function to parse an HTTP request
// int Request::parseHttpRequest(std::string requestBuffer, int new_socket, size_t bytesread) {
//     (void)new_socket;
//     int i = processChunk(requestBuffer, bytesread);
//     return i;
// }

// size_t Request::customFind(const std::vector<char>& vec, const std::string& str, size_t start)
// {
//     size_t vectorSize = vec.size();
//     size_t strLength = str.length();
//     if (strLength == 0 || start >= vectorSize) {
//         return std::string::npos;  // Return 'not found' if the input is invalid.
//     }
//     for (size_t i = start; i < vec.size(); ++i) {
//         if (vec[i] == str[0])
//         { // If the first character matches
//             bool found = true;
//             for (size_t j = 0; j < str.length(); ++j) {
//                 if (i + j >= vec.size() || vec[i + j] != str[j])
//                 {
//                     found = false;
//                     break;
//                 }
//             }
//             if (found)
//                 return i; // Found at position i
//         }
//     }
//     return std::string::npos; // Not found
// }

// std::vector<char> Request::customSubstr(const std::vector<char>& vec, size_t start, size_t length)
// {
//     std::vector<char> result;

//     if (start >= vec.size()) {
//         return result; // Return an empty vector if the start position is out of bounds
//     }
//     for (size_t i = start; i < start + length && i < vec.size(); ++i) {
//         result.push_back(vec[i]);
//     }
//     return result;
// }

// int Request::countNumbersInString(const std::string& inputString) {
//     int count = 0;
//     for (size_t i = 0; i < inputString.length(); ++i) {
//         if (std::isdigit(inputString[i])) {
//             count++;
//         }
//     }
//     return count;
// }

// int		Request::processChunk(std::string buffer, size_t bytesread)
// {
//     (void)bytesread;
//     if (!_headers_done)
//     {
//         _all.append(buffer);
//         size_t done = _all.find("\r\n\r\n");
//         if (done != std::string::npos)
//         {
//             _header_before = _all.substr(0, done);
//             _bodies = _all.substr(done + 4);
//             _headers_done = true;
//             int j = parseHeaders();
//             for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
//             {
//                 std::cout << it->first << " " << it->second << std::endl;
//             }
//             if (!j)
//             {
//                 _bodies.clear();
//                 return 0;
//             }
//         }
//     }
//     else
// 	{
//         _bodies.append(buffer);
//         return processBody();
//     }
//     return 1;
// }

// std::string Request::GenerateFile() {
//     std::string Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefh12326544";
//     std::string file;
    
//     // Create the directory if it doesn't exist //!!!!!!!!!!!!!!!!!!
//     const char* dir_path = "/nfs/homes/sben-ela/save/POSTs/";
//     if (mkdir(dir_path, 0777) != 0 && errno != EEXIST) {
//         std::cerr << "Failed to create directory: " << strerror(errno) << std::endl;
//         return "";  // Return an empty string to indicate failure
//     }
    
//     for (size_t i = 0; i < FILESIZE; i++) {
//         file += Base[rand() % Base.size()];
//     }
//     _ofile = dir_path + file;
//     return _ofile;
// }

// std::map<int, std::string> Request::getStatusCode( void ) const
// {
//     return(_statusCode);
// }

// void Request::CreateStatusCode()
// {
//     _statusCode[OK] = " 200 OK";
//     _statusCode[CREATED] = " 201 Created";
//     _statusCode[MOVEDPERMANENTLY] = " 301 Moved Permanently";
//     _statusCode[NOTALLOWED] = " 405 Method Not Allowed";
//     _statusCode[CONTENTTOLARGE] = " 413 Request Entity Too Large";
//     _statusCode[URLTOOLONGE] = " 414 Request-URI Too Long";
//     _statusCode[MOVEDTEMPORARILY] = " 302 Moved Temporarily";
//     _statusCode[USEPROXY] = " 305 Use Proxy";
//     _statusCode[MULTIPLECHOICES] = " 300 Multiple Choices";
//     _statusCode[NOCONTENT] = " 204 No Content";
//     _statusCode[RESETCONTENT] = " 205 Reset Content";
//     _statusCode[PARTIALCONTENT] = " 206 Partial Content";
//     _statusCode[SEEOTHER] = " 303 See Other";
//     _statusCode[NOTMODIFIED] = " 304 Not Modified";
//     _statusCode[TEMPORARYREDIRECT] = " 307 Temporary Redirect";
//     _statusCode[BADREQUEST] = " 400 Bad Request";
//     _statusCode[UNAUTHORIZED] = " 401 Unauthorized";
//     _statusCode[PAYMENTREQUIRED] = "402 Payment Required";
//     _statusCode[FORBIDDEN] = " 403 Forbidden";
//     _statusCode[NOTFOUND] = " 404 Not Found";
//     _statusCode[NOTACCEPTABLE] = " 406 Not Acceptable";
//     _statusCode[PROXYAUTHENTICATIONREQUIRED] = " 407 Proxy Authentication Required";
//     _statusCode[REQUESTTIMEOUT] = " 408 Request Time-out";
//     _statusCode[CONFLICT] = " 409 Conflict";
//     _statusCode[GONE] = " 410 Gone";
//     _statusCode[LENGTHREQUIRED] = " 411 Length Required";
//     _statusCode[PRECONDITIONFAILED] = " 412 Precondition Failed";
//     _statusCode[UNSUPPORTEDMEDIATYPE] = " 415 Unsupported Media Type";
//     _statusCode[REQUESTEDRANGENOTSATISFIABLE] = " 416 Requested range not satisfiable";
//     _statusCode[EXPECTATIONFAILED] = " 417 Expectation Failed";
//     _statusCode[INTERNALSERVERERROR] = " 500 Internal Server Error";
//     _statusCode[NOTIMPLEMENTED] = " 501 Not Implemented";
//     _statusCode[BADGATEWAY] = " 502 Bad Gateway";
//     _statusCode[SERVICEUNAVAILABLE] = " 503 Service Unavailable";
//     _statusCode[GATEWAYTIMEOUT] = " 504 Gateway Time-out";
//     _statusCode[HTTPVERSIONNOTSUPPORTED] = " 505 HTTP Version not supported";
// }

// int    Request::processBody()
// {
//     size_t crlf_pos = _bodies.find("\r\n");
//     if (crlf_pos != std::string::npos) {
//         std::string chunk_size_str = _bodies.substr(0, crlf_pos);
//         char* endptr;
//         int chunksize = strtoul(chunk_size_str.c_str(), &endptr, 16);
//         if (!chunksize)
//         {
//             std::cout << "Body ended ..." << std::endl;
//             close(_fd);
//             return 0;
//         }
//         if (_bodies.length() >= chunksize + 2 + crlf_pos + 2)
//         {
//             std::string str = _bodies.substr(crlf_pos + 2, chunksize);
//             write(_fd, str.c_str(), chunksize);
//             _bodies = _bodies.substr(chunksize + 2 + crlf_pos + 2);
//         }
//     }
//     return 1;
// }

// int    Request::parseHeaders()
// {
//     // std::string header = vectorCharToString(_header_before);
//     // std::cout << _header_before << std::endl;
//     // exit(0);
//     std::istringstream requestStream(_header_before);
//     std::string line;

//     // Read the first line (request line)
//     if (!std::getline(requestStream, line)) {
//         // Handle an empty or incomplete request
//         setResponseStatus("400 Bad Request");
//         return 0;
//     }
//     std::istringstream requestLineStream(line);
//     std::cout << "Line : " << line << std::endl;
//     if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
//         // Handle invalid request line
//         setResponseStatus("400 Bad Request");
//         return 0;
//     }
//     //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
//     // if (_path == "/favicon.ico") {
//     //     // Handle it as needed (status), or simply return an empty request
//     //     return 0;
//     // }
//     std::string forBody;
//     // Read and parse headers
//     while (std::getline(requestStream, line) && !line.empty()) {
//         forBody += line + "\n";
//         size_t pos = line.find(":");
//         if (pos != std::string::npos) {
//             std::string headerName = line.substr(0, pos);
//             std::string headerValue = line.substr(pos + 1);
//             // Remove leading/trailing whitespaces from header values
//             headerValue.erase(0, headerValue.find_first_not_of(" \t"));
//             headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
//             _headers[headerName] = headerValue;
//         }
//     }
//     if (_method == "GET")
//     {
//         close(_fd);
//         return 0;
//     }
//     else
//     {
//         _fd = open(GenerateFile().c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
//         if (_fd == -1) {
//             std::cerr << "Failed to open the file." << std::endl;
//             return 0;
//         }
//     }
//     return 1;
// }

// std::string Request::vectorCharToString(const std::vector<char>& vec)
// {
//     std::string result;
//     result.reserve(vec.size()); // Reserve space for efficiency
//     for (size_t i = 0; i < vec.size(); ++i) {
//         result.push_back(vec[i]);
//     }
//     return result;
// }

// Request::Request(const Request& other)
//     :   _method(other._method),
//         _path(other._path),
//         _httpVersion(other._httpVersion),
//         _responseStatus(other._responseStatus),
//         _header_before(other._header_before),
//         _body(other._body),
//         _headers(other._headers),
//         _statusCode(other._statusCode),
//         _contentType(other._contentType),
//         _bodyFile(other._bodyFile),
//         _ofile(other._ofile),
//         _chunksize(other._chunksize),
//         _bodies(other._bodies),
//         _all(other._all),
//         _headers_done(other._headers_done),
//         _fd(other._fd) {}

// Request& Request::operator=(const Request& other)
// {
//     if (this != &other)
//     {
//         _method = other._method;
//         _path = other._path;
//         _httpVersion = other._httpVersion;
//         _responseStatus = other._responseStatus;
//         _header_before = other._header_before;
//         _body = other._body;
//         _headers = other._headers;
//         _statusCode = other._statusCode;
//         _contentType = other._contentType;
//         _bodyFile = other._bodyFile;
//         _ofile = other._ofile;
//         _chunksize = other._chunksize;
//         _bodies = other._bodies;
//         _all = other._all;
//         _headers_done = other._headers_done;
//         _fd = other._fd;
//     }
//     return *this;
// }

// std::string         Request::GetFileExtention( void ) const
// {
//     std::string extention;
//     size_t index;

//     index = _path.find('.');
//     if (index == std::string::npos)
//         return(extention);
//     extention = _path.substr(index);
//     return (extention);
// }

// std::string         Request::getContentType( void ) const
// {
//     std::string type;

//     type = _path.substr(_path.find('.'));
//     return (type);
// }
// void                Request::setPath(std::string newPath)
// {
//     _path = newPath;
// }
// /*int Request::parseRequest()
// {
//     // std::cout << "----------------+++++++++++++++++++++++++++" << std::endl;
//     // std::cout << _header_before << std::endl;
//     // std::cout << "----------------+++++++++++++++++++++++++++" << std::endl;
//     // return 1;
//     std::istringstream requestStream(_header_before);
//     std::string line;

//     // Read the first line (request line)
//     if (!std::getline(requestStream, line)) {
//         // Handle an empty or incomplete request
//         setResponseStatus("400 Bad Request");
//         return 0;
//     }
//     std::istringstream requestLineStream(line);
//     if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
//         // Handle invalid request line
//         setResponseStatus("400 Bad Request");
//         return 0;
//     }
//     //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
//     if (_path == "/favicon.ico") {
//         // Handle it as needed (status), or simply return an empty request
//         return 0;
//     }
//     std::string forBody;
//     bool isContentLengthFound = false;
//     std::size_t contentLength = 0;
//     // Read and parse headers
//     while (std::getline(requestStream, line) && !line.empty()) {
//         forBody += line + "\n";
//         size_t pos = line.find(":");
//         if (pos != std::string::npos) {
//             std::string headerName = line.substr(0, pos);
//             std::string headerValue = line.substr(pos + 1);
//             // Remove leading/trailing whitespaces from header values
//             headerValue.erase(0, headerValue.find_first_not_of(" \t"));
//             headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
//             _headers[headerName] = headerValue;
//             if (_method == "POST" && headerName == "Content-Length")
//             {
//                 try {
//                     char* endptr;
//                     const char* headerValueCStr = headerValue.c_str();
//                     unsigned long parsedContentLength = strtoul(headerValueCStr, &endptr, 10);

//                     if (parsedContentLength == ULONG_MAX) { endptr == headerValueCStr || *endptr != '\0' ||
//                         // Handle invalid Content-Length value
//                         setResponseStatus("400 Bad Request");
//                         return 0;
//                     }
//                     contentLength = parsedContentLength;
//                     isContentLengthFound = true;
//                 } catch (const std::exception& e) {
//                     // Handle invalid Content-Length value
//                     setResponseStatus("400 Bad Request");
//                     return 0;
//                 }
//             }
//         }
//     }
//     // std::cout << "*******************" << std::endl;
//     // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
//     // {
//     //     std::cout << it->first << " " << it->second << std::endl;
//     // }
//     // std::cout << std::endl;
//     // std::cout << "*******************" << std::endl;
//     // (void)new_socket;
//     if (_method == "POST") {
//         if (!isContentLengthFound) {
//             // Handle missing Content-Length header for POST requests
//             setResponseStatus("411 Length Required");
//             return 0;
//         }
//         std::size_t bodyPos = forBody.find("\r\n\r\n");
//         if (bodyPos != std::string::npos) {
//             // Extract the body
//             std::string requestBody = forBody.substr(bodyPos + 4);
//             // Write the body to a file
//             _bodyFile = "BodyOfRequest.txt";
//             std::ofstream outfile(_bodyFile, std::ofstream::binary);
//             outfile.write(requestBody.c_str(), contentLength);
//             outfile.close();
//         }
//         else {
//             // Handle missing Content-Length header for POST requests
//             setResponseStatus("411 Length Required");
//             return 0;
//         }
//     }
//     setResponseStatus("200 OK");
//     return (0);
// }*/

// const std::string& Request::getPath() const
// {
//     return this->_path;
// }

// const std::string& Request::getMethod() const
// {
//     return this->_method;
// }

// const std::string& Request::getHttpVersion() const
// {
//     return this->_httpVersion;
// }

// const std::string& Request::getResponseStatus() const
// {
//     return this->_responseStatus;
// }

// void Request::setResponseStatus(const std::string& status) {
//     _responseStatus = status;
// }

// Request::~Request() {}