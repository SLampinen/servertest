NAME = client

CC = clang++
CFLAGS = -Wall -Wextra -Werror -g 
RM = rm -rf

INCLUDES = -I includes -I libft/includes

HEADERS	= incl/webserv.hpp

SRCS = src/clientMain.cpp

OBJS = $(SRCS:.cpp=.o)

all:
	@$(MAKE) $(NAME) -j5
$(NAME) : $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: 	fclean all

.PHONY: all clean fclean re