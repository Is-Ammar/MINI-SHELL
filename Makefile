CC = cc

CFLAGS = -Wall -Wextra -Werror -g

# -------------------------------- habdella -------------------------------- #

PARS_SRC = 	parsing/navigation_access.c parsing/basic_operations.c  parsing/tokenizer.c \
			parsing/tools/token_helpers.c parsing/parse_checkers.c parsing/parse_modifiers.c \
			parsing/tools/parse_helpers.c parsing/logic_check.c parsing/expand.c parsing/error_handler.c \
			parsing/tools/wcard_helpers.c parsing/wildcard.c parsing/heredoc.c \
			parsing/tools/hered_helpers.c parsing/redirections.c parsing/garbage_collector.c \
			parsing/tools/redir_helpers.c

# -------------------------------------------------------------------------- #

# --------------------------------- iammar --------------------------------- #

EXEC_SRC = 	execution/execution.c execution/utils.c execution/builtin/cd.c \
			execution/env/env_variable_settings.c execution/env/env_variable_settings1.c \
			execution/builtin/pwd.c execution/builtin/env.c execution/builtin/exit.c \
			execution/builtin/echo.c execution/builtin/export.c execution/builtin/helpers.c \
			execution/builtin/unset.c execution/abstract_syntax_tree/parse_simple_command.c execution/abstract_syntax_tree/ast.c \
			execution/abstract_syntax_tree/execute_ast.c execution/external_command/external_command.c \
			execution/utils2.c execution/execute_pipe/execute_pipe.c execution/abstract_syntax_tree/execute_command.c \
			execution/external_command/get_command_path.c

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
	${RM} ${PARS_OBJ} ${EXEC_OBJ} main.o

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: ${PARS_OBJ} ${EXEC_OBJ} main.o