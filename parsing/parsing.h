/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/17 13:52:36 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h> 
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../execution/execution.h"
# include "../smash.h"

# define RESET	 "\001\033[0m\002"
# define RED	 "\001\033[0;31m\002"
# define PURPLE  "\001\033[0;35m\002"
# define CYAN	 "\001\033[0;36m\002"

# define B_RED		"\001\033[1;31m\002"
# define B_GREEN	"\001\033[1;32m\002"
# define B_YELLOW	"\001\033[1;33m\002"
# define B_BLUE		"\001\033[1;34m\002"
# define B_PURPLE	"\001\033[1;35m\002"
# define B_CYAN		"\001\033[1;36m\002"
# define B_WHITE	"\001\033[1;37m\002"
# define B_GRAY		"\001\033[1;90m\002"

# define FALSE			0
# define TRUE			1
# define METACHARS		"( \t\n\v\f\r)<|&>'\"$"
# define WHITESPACES	" \t\n\v\f\r"
# define WS				WHITESPACES
# define LEFT			1
# define RIGHT			2

typedef enum e_error_type
{
	EQUOTES,
	EBRACKET,
	ESYNTAX,
	ECOMMAND,
	EDIRFILE,
	EPERMISS,
	EAMBIGUO,
	ENINTCMD,
}	t_error_type;

typedef enum e_operator
{
	AND = 1,
	OR = 2,
}	t_operator;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECTION,
	OPERATOR,
	SYMBOL,
	WHITESPACE,
}	t_token_type;

typedef enum e_quote_type
{
	NONE,
	SQUOTE,
	DQUOTE
}	t_quote_type;

typedef enum e_redirect
{
	READ = 1,
	WRITE = 2,
	APPEND = 3,
}	t_redirect;

typedef struct s_dll
{
	char			*value;
	t_token_type	token_type;
	t_quote_type	quote_type;
	t_redirect		redir_type;
	int				inside_parentheses;
	int				expandable;
	int				is_splited;
	int				direction;
	int				bracket;
	int				heredoc;
	int				expandoc;
	int				wildcard;
	int				operator;
	int				fake_cmd;
	int				level;
	struct s_dll	*prev;
	struct s_dll	*next;
}	t_dll;

typedef struct s_expand
{
	char	*str;
	char	*mask;
}	t_expand;

typedef struct s_gc
{
	void				*address;
	int					is_env;
	struct s_gc			*next;
}	t_gc;

typedef struct s_shell	t_shell;
typedef struct s_env	t_env;
typedef struct s_ast	t_ast;

// -------------------- Everything about tokens -------------------- //
/* ///////////////// Basic token operation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*create_token_list(t_shell *shell);
void	add_token(t_shell *shell, t_dll **head, char *val, t_token_type t_type);
void	add_mid_token(t_shell *shell, t_dll **head, t_dll *token, char *val);
void	remove_token(t_dll **head, t_dll *token);
void	free_node(t_gc **head, t_gc *remove);
/* ///////////////// token navigation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*find_token(t_dll *head, t_token_type token_type);
t_dll	*find_command(t_shell *shell, t_dll *head);
/* ///////////////// token & parsing modification \\\\\\\\\\\\\\\\\\\ */
char	*ft_merge(t_shell *shell, char *first_val, char *second_val);
void	merge_tokens(t_shell *shell, t_dll *first_token, t_dll *second_token);
t_dll	*tokenize_input(t_shell *shell, char *input);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */
int		get_token_type(char *val);
int		get_quote_type(char *val);
char	*get_token_val(t_shell *shell, char *input, int *index);
int		ft_strlen(const char *s);
char	*ft_strduplen(t_shell *shell, char *input, int len);
char	*env_strdup(t_shell *shell, const char *s);
char	*ft_strchr(const char *s, int c);
int		ft_printf(const char *format, ...);
char	*my_readline(t_shell *shell);
int		parse_error(char *val, t_error_type error);
int		exec_error(t_shell *shell, char *val, t_error_type error);
// ----------------------------------------------------------------- //

// -------------------- Everything about Parsing -------------------- //

/* ///////////////// main parsing functions \\\\\\\\\\\\\\\\\\\\\\\\\ */
int		parsing(t_shell *shell, char *input);
int		parse_input(t_shell *shell, t_dll **tokens);
/* ////////////////// checkers \\\\\\\\\\\\\\\\\\\\\ */
int		check_quotes(t_dll *tokens);
int		check_brackets(t_dll *tokens);
int		check_subshell(t_dll *tokens);
void	subshell_level(t_dll *tokens);
int		subshell_last(t_dll *tokens);
int		check_logic(t_dll *tokens);
int		additional_check_logic(t_dll *curr);
/* ///////////////// modifiers \\\\\\\\\\\\\\\\\\\\\ */
void	operators_merge(t_shell *shell, t_dll **tokens);
void	merge_quotes(t_shell *shell, t_dll **tokens);
void	remove_spaces(t_dll **tokens);
void	identify_tokens(t_dll *tokens);
/* ///////////////// expansion \\\\\\\\\\\\\\\\\\\\\ */
int		check_depth_to_expand(char *val);
char	*expand_env_str(t_shell *shell, char *value);
char	*double_quote(t_shell *shell, char *val, int *i);
char	*single_quote(t_shell *shell, char *value, int *i);
char	*dollar_sign(t_shell *shell, char *value, int *i, int is_dquote);
int		expansion(t_shell *shell, t_dll **tokens, t_dll **token);
int		expanding(t_shell *shell, t_dll **tokens, t_dll *curr);
t_dll	*add_to_tokens(t_shell *shell, t_dll **head, t_dll *token, char *val);
char	*get_mask_str(t_shell *shell, char *value);
t_dll	*split_tok(t_shell *shell, t_dll **tokens, t_dll *curr, t_expand *exp);
void	remove_additonal_chars(t_shell *shell, t_dll *token, t_dll *last_token);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */
int		ft_strlen_char127(const char *s);
char	*set_val(t_shell *shell, char *val, char c);
char	*remove_char127(t_shell *shell, char *token);
char	*ft_strdup_expand(t_shell *shell, char *value);
char	*ft_strjoin(t_shell *shell, char *s1, char *s2);
char	*ft_itoa(t_shell *shell, int n, int is_env);
/* ///////////////// wildcards \\\\\\\\\\\\\\\\\\\\\ */
int		wildcard(t_shell *shell, t_dll **tokens, t_dll *curr, char *old_val);
int		search_match(t_shell *shell, char *pattern, char *str, char *old_val);
int		pattern_evaluator(char *str, char *pattern, int **matrix, char *mask);
char	*get_mask_stars(t_shell *shell, char *pattern);
/* ///////////////// heredoc \\\\\\\\\\\\\\\\\\\\\ */
void	heredoc(t_shell *shell, t_dll **tokens);
void	handle_herdoc(t_shell *shell, t_dll *nxt, char *name);
void	open_heredoc(t_shell *shell, char *delim, char *name);
int		get_name_number(t_shell *shell, int count);
void	expand_heredoc(t_shell *shell, char *old_name);
char	*get_next_line(t_shell *shell, int fd);
char	*expand_in_heredoc(t_shell *shell, char *value);
char	*ft_strnstr(const char *big, const char *little, int len);
char	*expand_delim(t_shell *shell, char *value);
char	*delim_dquote(t_shell *shell, char *val, int *i);
char	*delim_dollar(t_shell *shell, char *value, int *i, int is_dquote);
/* ///////////////// redirections \\\\\\\\\\\\\\\\\\\\\ */
void	redirect(t_dll **tokens);
int		handle_redirect(char *value, t_dll *nxt);
void	identify_redirections(t_dll **tokens);
int		in_fd(t_shell *shell, t_dll **tokens, t_dll *token);
int		out_fd(t_shell *shell, t_dll **tokens, t_dll *token, int O_FLAG);
// ------------------------------------------------------------------ //
/* ///////////////// address flushers \\\\\\\\\\\\\\\\\\\\\ */
void	*ft_malloc(t_shell *shell, size_t size, int is_env);
void	add_to_garbage(t_shell *shell, void *ptr, int is_env);
void	burn_garbage(t_shell *shell);
void	clear_non_env(t_shell *shell);
void	clean_exit(t_shell *shell, int exit_code);
/* ////////////////////////// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */

#endif