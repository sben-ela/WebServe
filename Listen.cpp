#include "Config.hpp"
void PrintBool(bool value) ;
size_t countChar(const std::string& str, char targetChar)
{
    size_t count = 0;

	for (size_t i = 0; str[i]; i++)
		if (str[i] == targetChar)
			count++;

    return (count);
}

bool	StrIsDigit(const std::string& str)
{
	for (size_t i = 0; isdigit(str[i]); i++)
	{
		if (!str[i + 1])
			return (true);
	}
	return (false);
}

bool   Listen::IsPort(const std::string &str) const
{
	return(StrIsDigit(str) ? true : false);
}

bool   Listen::IsIp(const std::string &str) const
{
	return(StrIsDigit(str) ? false : true);
}


size_t GetIndex(std::string &str)
{
	size_t index = str.find("	listen		");
	if (index == std::string::npos)
		throw (std::runtime_error("Invalid Listen"));
	return(index + 9);
}

Listen::Listen(std::string &listen){
    size_t index = GetIndex(listen);
    size_t found = listen.find(':');
	if (found == std::string::npos)
		exit(1);

	std::string left	= listen.substr(index, found - index);
	std::string right	= listen.substr(found + 1);
	if (left.empty() && !right.empty())
		_status = (IsPort(right) ? listenPORT : listenIP);	
	else if (!left.empty() && right.empty())
		_status = (IsPort(left) ? listenPORT : listenIP);
	else if (IsIp(left) && IsPort(right))
		_status = IPPORT;
	else if (IsPort(left) && IsIp(right))
		_status = PORTIP;
	else
		throw (std::runtime_error("Invalid Listen"));
	
}

void PrintBool(bool value) {
    if (value) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }
}

int main()
{
	try{
		std::string str = "	listen		*:80";
		Listen test(str);
		test.info();
	}
	catch(std::exception Error)
	{
		std::cout << Error.what() << std::endl;
	}

}