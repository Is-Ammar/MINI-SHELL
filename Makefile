CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = 	main.c basic_operations.c navigation_access.c token_helpers.c tokenizer.c \
		parser.c error_handler.c

OBJ = ${SRC:%.c=%.o}

RM = rm -f

NAME = minishell

all: ${NAME}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJ}
	${CC} ${CFLAGS} -g -lreadline ${SRC} -o $@

clean:
	${RM} ${OBJ}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: ${OBJ}