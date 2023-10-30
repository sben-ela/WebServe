NAME = webserv
CC = c++
RM = rm -f
CPPFLAGS =  -std=c++98
IMPLEMENTATIONS = Request.cpp Response.cpp Client.cpp 
CONFIG = Servers.cpp Configuration.cpp Location.cpp
SRCS = $(addprefix ./Config/, $(CONFIG)) \
		$(addprefix ./Implementations/, $(IMPLEMENTATIONS)) \
		main.cpp
OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) data/Post/*

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)