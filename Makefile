NAME = webserv
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
IMPLEMENTATIONS = Socket.cpp ConfigFile.cpp Request.cpp
SRCS = $(addprefix ./Implementations/, $(IMPLEMENTATIONS)) \
		main.cpp
OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
	$(RM) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)