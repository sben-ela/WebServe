NAME = webserv
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3 
IMPLEMENTATIONS = Socket.cpp Request.cpp Response.cpp 
CONFIG = Servers.cpp Configuration.cpp Location.cpp
SRCS = $(addprefix ./Config/, $(CONFIG)) \
		$(addprefix ./Implementations/, $(IMPLEMENTATIONS)) \
		main.cpp  Response/Client.cpp Response/Response.cpp
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