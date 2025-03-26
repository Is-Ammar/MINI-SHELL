CC = cc

CFLAGS = -Wall -Wextra -Werror -g

# -------------------------------- habdella -------------------------------- #

PARS_SRC = 	parsing/basic_operations.c parsing/navigation_access.c parsing/token_helpers.c \
			parsing/tokenizer.c parsing/parse_checkers.c parsing/parse_modifiers.c \
			parsing/error_handler.c 

# -------------------------------------------------------------------------- #

# --------------------------------- iammar --------------------------------- #

EXEC_SRC = 	execution/execution.c execution/utils.c execution/builtin/cd.c \
			execution/env/env_variable_settings.c execution/env/create_env.c \
			execution/builtin/pwd.c execution/builtin/env.c execution/builtin/exit.c \
			execution/builtin/echo.c execution/builtin/export.c

# -------------------------------------------------------------------------- #
PARS_OBJ = ${PARS_SRC:%.c=%.o}

EXEC_OBJ = ${EXEC_SRC:%.c=%.o}

RM = rm -f

NAME = minishell

all: ${NAME}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${PARS_OBJ} ${EXEC_OBJ} main.o
	${CC} ${CFLAGS} ${PARS_OBJ} ${EXEC_OBJ} main.o -lreadline -o $@

clean:
	${RM} ${PARS_OBJ} ${EXEC_OBJ}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: ${PARS_OBJ} ${EXEC_OBJ} main.o