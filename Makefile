NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = src/coder.c \
	src/dongle.c \
	src/input_check.c \
	src/main.c \
	src/monitor.c \
	src/my_atoi.c \
	src/quantum_compiler.c \
	src/safePrint.c \
	src/sim.c \
	src/time.c

all:
	$(CC) $(SRC) -o $(NAME)

compile:
	$(CC) $(CFLAGS) -pthread -g -o $(NAME) $(SRC)

clean:
	rm src/codexion.h.gch

fclean:
	rm src/codexion.h.gch
	rm codexion

re:
	