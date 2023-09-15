#include <iostream>
#include <fstream>
#include <string>

bool serve_file(const std::string& request_uri, const std::string& server_root) {
    // Construct the full path to the requested file
    std::string full_path = server_root + request_uri;

    // Open the file
    std::ifstream file(full_path);
    if (file.is_open()) {
        // File exists, read its content and send as the response
        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::cout << "200 OK\n\n" << file_content << std::endl;
        return true;
    } else {
        // File not found
        std::cout << "404 Not Found" << std::endl;
        return false;
    }
}

int main() {
    std::string request_uri = "/example.html"; // Replace with the actual request URI
    std::string server_root = "/path/to/your/server/root"; // Set the server's root directory

    if (!serve_file(request_uri, server_root)) {
        // Return a 404 Not Found response if the file is not found
        std::cout << "404 Not Found" << std::endl;
    }

    return 0;
}
