# below is correct version in order to avoid unnesessary relinking
# and there is a rule for NAME

NAME = codexion

CC = cc
CFLAGS = -Fsanitize=thread -Wall -Wextra -Werror -pthread -g -o
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

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(NAME) $(SRC)

clean:
	rm codexion

fclean: clean

re: fclean all

.PHONY: all clean fclean re