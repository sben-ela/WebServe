#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

class Request {
public:
    std::string _method;
    std::string _path;
    std::string _httpVersion;
    std::map<std::string, std::string> _header;
    std::string _body;
};

Request parseHttpRequest(const std::string& requestBuffer) {
    Request request;
    std::istringstream requestStream(requestBuffer);
    std::string line;

    // Read the first line (request line)
    if (std::getline(requestStream, line)) {
        std::istringstream requestLineStream(line);
        requestLineStream >> request._method >> request._path >> request._httpVersion;
    }

    // Read and parse headers
    while (std::getline(requestStream, line) && !line.empty()) {
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            request._header[headerName] = headerValue;
        }
    }

    // Check if the request has a Content-Length header
    if (request._header.find("Content-Length") != request._header.end()) {
        int contentLength = atoi(request._header["Content-Length"].c_str());
        char buffer[contentLength];
        requestStream.read(buffer, contentLength);
        request._body = std::string(buffer, contentLength);

        // Save the request body to a file
        std::ofstream outputFile("request_body.txt");
        if (outputFile.is_open()) {
            outputFile.write(buffer, contentLength);
            outputFile.close();
        }
    }

    return request;
}

int main() {
    std::string requestBuffer = "POST /index.txt HTTP/1.1\n"
                                "Content-Type: text/plain\n"
                                "User-Agent: PostmanRuntime/7.32.3\n"
                                "Accept: */*\n"
                                "Postman-Token: 61c5d83a-3e4c-445f-8144-afeb9ce15cd7\n"
                                "Host: localhost:8080\n"
                                "Accept-Encoding: gzip, deflate, br\n"
                                "Connection: keep-alive\n"
                                "Content-Length: 21\n"
                                "\n"
                                "Hello thbrsthwrthwrth";

    Request parsedRequest = parseHttpRequest(requestBuffer);

    // Print the parsed request
    std::cout << "Method: " << parsedRequest._method << std::endl;
    std::cout << "Path: " << parsedRequest._path << std::endl;
    std::cout << "HTTP Version: " << parsedRequest._httpVersion << std::endl;

    std::cout << "Headers:" << std::endl;
    for (const auto& header : parsedRequest._header) {
        std::cout << header.first << ": " << header.second << std::endl;
    }

    std::cout << "Body: " << parsedRequest._body << std::endl;

    return 0;
}
