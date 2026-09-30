NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g -o
SRC = src/coder.c \
	src/dongle.c \
	src/input_check.c \
	src/main.c \
	src/monitor.c \
	src/my_atoi.c \
	src/quantum_compiler.c \
	src/safe_print.c \
	src/sim.c \
	src/time.c \
	src/burn_out_check.c \
	src/routine_steps.c \
	src/queue.c \
	src/queue_init.c \
	src/dongle_acquisition.c \
	src/thread_creation.c \
	src/scheldule.c \
	src/deadline.c
all:
	$(CC) $(CFLAGS) $(NAME) $(SRC)

clean:
	rm src/codexion.h.gch

fclean:
	rm src/codexion.h.gch
	rm codexion

re:
	