NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = src

all:
	$(CC) SRC/. -o NAME

clean:

fclean:

re: