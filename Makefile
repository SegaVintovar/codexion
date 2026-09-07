NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = src

all:
	$(CC) SRC/. -o NAME

compile:
	cc -Wall -Wextra -pthread -o NAME src/*.c

clean:
	rm src/codexion.h.gch

fclean:

re: