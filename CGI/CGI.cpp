#include "CGI.hpp"

void    Cgi::CgiExecute()
{
	int fd[2];
	char *Path[3] = {(char *)_path.c_str(), (char *)_fileName.c_str(), NULL};

	int pid  = fork();
	pipe(fd);
	if (!pid)
	{
		dup2(1, fd[1]);
		execve(Path[0], Path, 0);
	}
	waitpid(pid, 0, 0);
	char buff[100];
	read(fd[1], (char *)_output.c_str() , 100);
	std::cout << _output << std::endl;
}

int main()
{
	Cgi test("/usr/bin/php-cgi", "test.php");
	test.CgiExecute();
}