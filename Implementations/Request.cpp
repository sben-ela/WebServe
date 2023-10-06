/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/10/05 22:33:39 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Request.hpp"

Request::Request() : _transferEncodingChunked(false), _transferEncoding(false), _contentLength(false), _headers_done(false){
    _responseStatus = 0;
}

const int&  Request::getFd() const
{
    return _fd;
}

// Function to parse an HTTP request
int Request::parseHttpRequest(std::string requestBuffer) {
    return processChunk(requestBuffer);
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

int		Request::processChunk(std::string buffer)
{
    if (!_headers_done)
    {
        _all.append(buffer);
        size_t done = _all.find("\r\n\r\n");
        if (done != std::string::npos)
        {
            _header_before = _all.substr(0, done);
            _bodies = _all.substr(done + 4);
            _headers_done = true;
            if (!parseHeaders())
            {
                std::cout << _method << "|" << _path << "|" << _httpVersion << "|" << std::endl;
                for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
                {
                    std::cout << it->first << " " << it->second << std::endl;
                }
                _bodies.clear();
                return 0;
            }
            // else
            // {
            //     return processBody();
            // } // ! in case makanch chunked khass dar liha function boo7dha  tkhchi dakchi f file
        }
    }
    else
	{
        _bodies.append(buffer);
        return processBody();
    }
    return 1;
}

std::string GenerateRandomString(int length) {
    const char* Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefh12326544";
    std::string randomString;
    for (int i = 0; i < length; i++) {
        randomString += Base[rand() % strlen(Base)];
    }
    return randomString;
}

std::string GenerateTimestamp() {
    time_t currentTime;
    struct tm* localTimeInfo;
    char timestamp[20];

    time(&currentTime);
    localTimeInfo = localtime(&currentTime);

    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localTimeInfo);

    return timestamp;
}

std::string Request::GenerateFile() {
    std::string randomString = GenerateRandomString(6); // 6 characters for the filename
    std::string timestamp = GenerateTimestamp();

    const char* dir_path = "/nfs/sgoinfre/goinfre/Perso/sben-ela/";

    if (mkdir(dir_path, 0777) != 0 && errno != EEXIST) {
        std::cerr << "Failed to create directory: " << strerror(errno) << std::endl;
        return "";  // Return an empty string to indicate failure
    }

    std::string fileName = dir_path + timestamp + "_" + randomString;
    _ofile = fileName;
    return _ofile;
}

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
            // close(_fd); // !!!!
            setResponseStatus(200);
            return 0;
        }
        if (_bodies.length() >= chunksize + 2 + crlf_pos + 2)
        {
            std::string str = _bodies.substr(crlf_pos + 2, chunksize);
            write(_fd, str.c_str(), chunksize);
            _bodies = _bodies.substr(chunksize + 2 + crlf_pos + 2);
            if (_bodies == "0\r\n\r\n")
            {
                setResponseStatus(200);
                return 0;
            }
        }
    }
    return 1;
}

int    Request::parseHeaders()
{
    std::istringstream requestStream(_header_before);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus(400);
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus(400);
        return 0;
    }
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return 0;
    }
    if (_path.length() > 2048)
    {
        setResponseStatus(414);
        return 0;
    }
    if (!_path.empty())
    {
        while (1)
        {
            size_t found = _path.find("%");
            if (found != std::string::npos) {
                std::string firstPart = _path.substr(0, found);
                std::string secondPart = _path.substr(found + 3);
                std::string number = _path.substr(found + 1, found + 3);
                long int value = strtoul(number.c_str(), NULL, 16);
                if (value >= 0 && value <= 255) {
                    char character = static_cast<char>(value);
                    firstPart += character + secondPart;
                    _path.clear();
                    _path = firstPart;
                }
                else
                {
                    std::cerr << "Invalid value of hexadecimal !" << std::endl;
                    return 0;
                }
            }
            else
                break ;
        }
        size_t found = _path.find("?");
        if (found != std::string::npos) {
            _queryString = _path.substr(found + 1);  // Get the substring after the '?'
            _path = _path.substr(0, found);  // Get the substring before the '?'
        }
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    std::string forBody;
    // Read and parse headers
    // bool inPart = false;
    std::string partContent;

    while (std::getline(requestStream, line) && !line.empty()) {
        forBody += line + "\n";
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            _headers[headerName] = headerValue; // ! nkhchiha with the boundary wla la
            if (headerName == "Content-Type")
                _contentTypeValue = headerValue;
        //     if (headerName == "Content-Type" && !strncmp(headerValue.c_str(), "multipart/form-data;", 19))
        //     {
        //         std::cout << "ATTENTION : " << std::endl;
        //         std::string boundary;
        //         size_t boundaryPos = headerValue.find("boundary="); // Find the position of "boundary="
        //         if (boundaryPos != std::string::npos)
        //             boundary = headerValue.substr(boundaryPos); // Extract the boundary value
        //         else
        //         {
        //             std::cerr << "Boundary not found in Content-Type." << std::endl; // Handle the case where "boundary=" is not found
        //             return 1; // ! THROW EXCEPTION??
        //         }
        //         while (std::getline(requestStream, line))
        //         {
        //             if (line == ("--" + boundary + "\r"))
        //             {
        //                 if (!inPart)
        //                     inPart = true;
        //                 else
        //                 {
        //                     // Process the current part
        //                     processMultiPart(partContent);
        //                     // Reset for the next part
        //                     partContent.clear();
        //                 }
        //             }
        //             else if (line == ("--" + boundary + "--\r"))
        //                 break ; // End of multipart data
        //             else if (inPart)
        //                 partContent += line + "\n"; // Append data to the current part
        //         }
        //         // Process the last part
        //         if (!partContent.empty())
        //             processMultiPart(partContent);
        //         setResponseStatus(200);
        //         return 0;
        //     }
            if (headerName == "Transfer-Encoding")
                _transferEncoding = true;
            if (headerName == "Content-Length")
                _contentLength = true;
            if (headerName == "Transfer-Encoding" && headerValue != "chunked\r")
                _transferEncodingChunked = true;
        // }
    }
    }
    if (_transferEncodingChunked)
    {
        setResponseStatus(501);
        return 0;
    }
    if (!_transferEncoding && !_contentLength && _method == "POST")
    {
        setResponseStatus(400);
        return 0;
    }
    if (_method == "GET" || _method == "DELETE")
    {
        close(_fd);
        setResponseStatus(200);
        return 0;
    }
    else if (_method == "POST")
    {
        std::string extension = ft_temp(); /// ! bdelha
        std::string name = GenerateFile() + "." + extension;
        _fd = open(name.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
        std::cout << "File created with number : " << _fd << std::endl;
        if (_fd == -1) {
            std::cerr << "Failed to open the file." << std::endl;
            return 0;
        }
    }
    else
    {
        setResponseStatus(501);
        return 0;
    }
    return 1;
}

void    Request::processMultiPart(std::string content)
{
    std::istringstream requestStream(content);
    std::string line;
    while (std::getline(requestStream, line) && !line.empty()) {
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            // _headers[headerName] = headerValue;
        }
        if (line == "\r")
        {
            ;
            //dkhlna fl body
            // ! if chunked and if not
        }
    }
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

std::string     Request::getQueryString() const
{
    if (!_queryString.empty())
        return _queryString;
    return ("");
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
        _queryString(other._queryString),
        _transferEncodingChunked(other._transferEncodingChunked),
        _transferEncoding(other._transferEncoding),
        _contentLength(other._contentLength),
        _headers_done(other._headers_done),
        _fd(other._fd),
        _contentTypeValue(other._contentTypeValue) {}

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
        _queryString = other._queryString;
        _transferEncodingChunked = other._transferEncodingChunked;
        _transferEncoding = other._transferEncoding;
        _contentLength = other._contentLength;
        _headers_done = other._headers_done;
        _fd = other._fd;
        _contentTypeValue = other._contentTypeValue;
    }
    return *this;
}

// ! hadi hya lihyedt
std::string         Request::ft_temp( void ) const
{
    std::string extention;
    size_t index;

    index = _contentTypeValue.find('/');
    if (index == std::string::npos)
        return(extention);
    extention = _contentTypeValue.substr(index + 1, _contentTypeValue.length() - index - 2);
    return (extention);
}

std::string Request::GetFileExtention( void ) const
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

int Request::getResponseStatus() const
{
    return this->_responseStatus;
}

void Request::setResponseStatus(int status) {
    _responseStatus = status;
}

Request::~Request() {}
/*
int    Request::parseHeaders()
{
    std::istringstream requestStream(_header_before);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus(400);
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus(400);
        return 0;
    }
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return 0;
    }
    if (_path.length() > 2048)
    {
        setResponseStatus(414);
        return 0;
    }
    if (!_path.empty())
    {
        size_t found = _path.find("?");
        if (found != std::string::npos) {
            _path = _path.substr(0, found);  // Get the substring before the '?'
            _queryString = _path.substr(found + 1);  // Get the substring after the '?'
        }
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
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
            if (headerName == "Transfer-Encoding")
                _transferEncoding = true;
            if (headerName == "Content-Length")
                _contentLength = true;
            if (headerName == "Transfer-Encoding" && headerValue != "chunked")
                _transferEncodingChunked = true;
        }
    }
    if (_transferEncodingChunked)
    {
        setResponseStatus(501);
        return 0;
    }
    if (!_transferEncoding && !_contentLength && _method == "POST")
    {
        setResponseStatus(400);
        return 0;
    }
    if (_method == "GET" || _method == "DELETE")
    {
        close(_fd);
        setResponseStatus(200);
        return 0;
    }
    else if (_method == "POST")
    {
        _fd = open(GenerateFile().c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
        std::cout << "File created with number : " << _fd << std::endl;
        if (_fd == -1) {
            std::cerr << "Failed to open the file." << std::endl;
            return 0;
        }
    }
    else
    {
        setResponseStatus(501);
        return 0;
    }
    return 1;
} saved version
*/