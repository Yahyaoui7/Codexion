NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g 

SRC = cleanup.c init.c main.c parsing.c coder.c logging.c  take_dongle.c monitor.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re



