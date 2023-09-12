#include "../Webserv.hpp"


class Cgi
{
    private :
        std::string	_path;
        std::string	_output;
		std::string	_fileName;
        short		_fileExtension;
    public :
        Cgi(std::string path, std::string fileName)
		{
            _path = path;
			_fileName = fileName;
        }
        void    CgiExecute();
};