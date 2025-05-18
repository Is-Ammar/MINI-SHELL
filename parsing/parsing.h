/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/18 11:27:38 by habdella         ###   ########.fr       */
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
# include "../smash.h"
# include "../execution/execution.h"

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

# define FALSE	0
# define TRUE	1
# define METACHARS	"( )\t\n<|&>'\"$"
# define LEFT	1
# define RIGHT	2

typedef enum e_error_type
{
	EQUOTES,
	EBRACKET,
	ESYNTAX,
	ECOMMAND,
	EDIRFILE,
	EPERMISS,
	EAMBIGUO,
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
	int				direction;
	int				bracket;
	int				heredoc;
	int				wildcard;
	int				operator;
	struct s_dll	*prev;
	struct s_dll	*next;
}	t_dll;

typedef struct s_gc
{
	void				*address;
	struct s_gc			*next;
}	t_gc;

typedef struct s_shell	t_shell;
typedef struct s_env	t_env;
typedef struct s_ast	t_ast;

// -------------------- Everything about tokens -------------------- //

/* ///////////////// Basic token operation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*create_token_list(t_shell *shell);
void	free_token_list(t_dll **head);
void	remove_token(t_dll **head, t_dll *token);
void	add_mid_token(t_shell *shell, t_dll **head, t_dll *token, char *val);
void	add_token(t_shell *shell, t_dll **head, char *val, t_token_type t_type);
/* ///////////////// token navigation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*find_token(t_dll *head, t_token_type token_type);
t_dll	*find_command(t_shell *shell, t_dll *head);
/* ///////////////// token & parsing modification \\\\\\\\\\\\\\\\\\\ */
char	*ft_merge(t_shell *shell, char *first_val, char *second_val);
void	merge_tokens(t_shell *shell, t_dll *first_token, t_dll *second_token);
t_dll	*tokenize_input(t_shell *shell, char *input);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */
t_token_type	get_token_type(char *val);
t_quote_type	get_quote_type(char *val);
char	*get_token_val(t_shell *shell, char *input, int *index);
int		ft_strlen(const char *s);
int		ft_strlen_quotes(const char *s);
int		ft_strlen_spaces(char *val, int *index);
char	*ft_strduplen(t_shell *shell, char *input, int len);
char	*ft_strdup_quotes(t_shell *shell, char *token);
char	*remove_quotes(t_shell *shell, char *token);
char	*ft_strchr(const char *s, int c);
int		ft_printf(const char *format, ...);
int		parse_error(char *val, t_error_type error);
int		exec_error(char *val, t_error_type error);
// ----------------------------------------------------------------- //

// -------------------- Everything about Parsing -------------------- //

/* ///////////////// main parsing function \\\\\\\\\\\\\\\\\\\\\\\\\ */
int		parse_input(t_shell *shell, t_dll **tokens);
/* ////////////////// checkers \\\\\\\\\\\\\\\\\\\\\ */
int		check_quotes(t_dll *tokens);
int		check_brackets(t_dll *tokens);
int		check_subshell(t_dll *tokens);
int		subshell_last(t_dll *tokens);
int		check_logic(t_dll *tokens);
/* ///////////////// modifiers \\\\\\\\\\\\\\\\\\\\\ */
void	operators_merge(t_shell *shell, t_dll **tokens);
void	merge_quotes(t_shell *shell, t_dll **tokens);
void	remove_spaces(t_dll **tokens);
void	identify_tokens(t_dll *tokens);
char	*expand_env_vars(t_shell *shell, char *value);
char	*double_quote(t_shell *shell, char *val, int *i);
char	*single_quote(t_shell *shell, char *value, int *i);
char	*dollar_sign(t_shell *shell, char *value, int *i, int is_dquote);
int		expansion(t_shell *shell, t_dll **tokens, t_dll **token);
int		expand_execute(t_shell *shell, t_dll **tokens, t_dll *curr);
void	split_token(t_shell *shell, t_dll **tokens, t_dll *curr, char *input);
t_dll	*add_to_tokens(t_shell *shell, t_dll **head, t_dll *token, char *val);
int		check_depth_to_expand(char *val);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */
char	*ft_strdup_expand(t_shell *shell, char *value);
char	*ft_strjoin(t_shell *shell, char *s1, char *s2);
char	*ft_itoa(t_shell *shell, int n);
int		should_be_splited(char *value);
int		is_empty(char *val);
/* ///////////////// wildcards \\\\\\\\\\\\\\\\\\\\\ */
int		wildcard(t_shell *shell, t_dll **tokens, t_dll *curr);
int		hidden_files(char *val, char *name);
int		search_for_match(t_shell *shell, t_dll *curr, char *val, char *d_name);
int		prefix(char *name, char *val, int *start);
int		infix(char *val, char *name, int i);
int		last_infix_check(char *val, char *name, int i, int j);
int		suffix(char *name, char *val);
/* ///////////////// heredoc \\\\\\\\\\\\\\\\\\\\\ */
void	heredoc(t_shell *shell, t_dll **tokens);
void	handle_herdoc(t_shell *shell, t_dll *nxt, char *name);
void	open_heredoc(t_shell *shell, char *delim, char *name);
void	expand_heredoc(t_shell *shell, char *old_name);
char	*get_next_line(t_shell *shell, int fd);
char	*expand_in_heredoc(t_shell *shell, char *value);
char	*ft_strnstr(const char *big, const char *little, int len);
void	last_check_doc(t_dll **tokens);
/* ///////////////// redirections \\\\\\\\\\\\\\\\\\\\\ */
void	redirect(t_dll **tokens);
int		handle_redirect(char *value, t_dll *nxt);
int		redirections(t_shell *shell, t_dll **tokens);
void	identify_redirections(t_dll **tokens);
int		in_fd(t_shell *shell, t_dll **tokens, t_dll *token);
int		out_fd(t_shell *shell, t_dll **tokens, t_dll *token, int O_FLAG);
// ------------------------------------------------------------------ //
/* ///////////////// address flushers \\\\\\\\\\\\\\\\\\\\\ */
void	*ft_malloc(t_shell *shell, size_t size);
void	add_to_garbage(t_shell *shell, void *ptr);
void	burn_garbage(t_shell *shell);
/* ////////////////////////// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */

#endif