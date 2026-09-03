NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = src

all:
	$(CC) SRC/. -o NAME

compile:
	cc src/* -g -pthread

clean:
	rm src/codexion.h.gch

fclean:

re: