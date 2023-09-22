#include <iostream>
#include <string>
#include <cctype>

// bool IsWhitespace(const std::string& line) {
//     for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
//         if (!isspace(*it)) {
//             return false; // If any non-whitespace character is found, return false
//         }
//     }
//     return true; // All characters are whitespace
// }

// int main() {
//     std::string line1 = "    "; // Contains only spaces
//     std::string line2 = "\t\t"; // Contains only tabs
//     std::string line3 = "  Hello  "; // Contains a mix of spaces and non-spaces

//     std::cout << "Line 1 is whitespace: " << IsWhitespace(line1) << std::endl;
//     std::cout << "Line 2 is whitespace: " << IsWhitespace(line2) << std::endl;
//     std::cout << "Line 3 is whitespace: " << IsWhitespace(line3) << std::endl;

//     return 0;
// }

std::string getFileName(const std::string& path)
{
    return (path.substr(path.find('/', 1) + 1));
}

std::string getFilePath()
{
    std::string filePath;
    
	filePath = "/nfs/homes/aybiouss/Desktop/Webserv/" + getFileName("/media/image.png");
	return (filePath);
}

int main()
{
    std::cout << getFilePath() << std::endl;
}