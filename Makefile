NAME = webserv
CC = c++
RM = rm -f
CPPFLAGS =  -std=c++98  -fsanitize=address 
IMPLEMENTATIONS = Request.cpp Response.cpp Client.cpp main.cpp 
CONFIG = Servers.cpp Configuration.cpp Location.cpp
SRCS = $(addprefix ./Config/, $(CONFIG)) \
		$(addprefix ./Implementations/, $(IMPLEMENTATIONS)) \

OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)