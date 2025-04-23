/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/23 09:36:11 by habdella         ###   ########.fr       */
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

#define RED     "\033[31m"				// Red text
#define RESET   "\001\033[0m\002"		// Reset all attributes
#define CYAN	"\001\033[0;36m\002"	// Cyan text
#define	PURPLE	"\001\033[0;35m\002"	// purple text

# define FALSE	0
# define TRUE	1
# define METACHARS	"( )\t\n<|&>'\"$"
# define LEFT	1
# define RIGHT	2

# define FD_IN	0
# define FD_OUT	1

typedef	enum e_error_type
{
	EQUOTES,
	EBRACKET,
	ESYNTAX,
	ECOMMAND
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

typedef	enum e_redirect
{
	READ = 1,
	WRITE = 2,
	APPEND = 3,
}	t_redicrect;

typedef struct s_dll
{
	char			*value;
	t_token_type 	token_type;
	t_quote_type	quote_type;
	t_redicrect		redir_type;
	int				fds[2];
	int				inside_parentheses;
	int				expandable;
	int				direction;
	int				bracket;
	int				heredoc;
	int				wildcard;
	int				operator;
	struct s_dll	*prev;
	struct s_dll	*next;
}  	t_dll;

typedef struct s_collector
{
	void				*address;
	struct s_collector	*next;
}	t_collector;


typedef struct s_shell	t_shell;
typedef	struct s_env	t_env;
typedef	struct s_ast	t_ast;

// -------------------- Everything about tokens -------------------- //

/* ///////////////// Basic token operation \\\\\\\\\\\\\\\\\\\\\ */ 
t_dll	*create_token_list(void);
void	free_token_list(t_dll **head);
void	remove_token(t_dll **head, t_dll *token);
void	add_mid_token(t_dll **head, t_dll *token, char *val);
void	add_token(t_dll **head, char *val, t_token_type token_type);
/* ///////////////// token navigation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*find_token(t_dll *head, t_token_type token_type);
/* ///////////////// token & parsing modification \\\\\\\\\\\\\\\\\\\ */
char	*ft_merge(char *first_val, char *second_val);
void	merge_tokens(t_dll *first_token, t_dll *second_token);
t_dll	*tokenize_input(char *input);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */ 
t_token_type	get_token_type(char *val);
t_quote_type	get_quote_type(char *val);
char			*get_token_val(char *input, int *index);
int				ft_strlen(const char *s);
int				ft_strlen_quotes(const char *s);
char			*ft_strduplen(char *input, int len);
char			*ft_strdup_quotes(char	*token);
char			*remove_quotes(char *token);
char			*ft_strchr(const char *s, int c);
void			Error(char *val, t_error_type error);
int				ft_printf(const char *format, ...);
// ----------------------------------------------------------------- //

// -------------------- Everything about Parsing -------------------- //

/* ///////////////// main parsing function \\\\\\\\\\\\\\\\\\\\\\\\\ */ 
int		parse_input(t_dll **tokens);
/* ////////////////// checkers \\\\\\\\\\\\\\\\\\\\\ */ 
int		check_quotes(t_dll *tokens);
int		check_brackets(t_dll *tokens);
int		check_subshell(t_dll *tokens);
int		subshell_last(t_dll *tokens);
int		check_logic(t_dll *tokens);
/* ///////////////// modifiers \\\\\\\\\\\\\\\\\\\\\ */ 
void	operators_merge(t_dll **tokens);
void	merge_quotes(t_dll **tokens);
void	remove_spaces(t_dll **tokens);
void	identify_tokens(t_dll *tokens);
char	*expand_env_vars(char *value, t_env *env, int e_code);
char	*double_quote(char *val, int *i, t_env *env, int e_code);
char	*single_quote(char *value, int *i);
char	*dollar_sign(char *value, int *i, t_env *env, int e_code);
int		expansion(t_dll **tokens, t_dll *curr, t_env *env, int e_code);
int		check_depth_to_expand(char *val);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */ 
char	*ft_strdup_expand(char *token, t_env *env);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_itoa(int n);
/* ///////////////// wildcards \\\\\\\\\\\\\\\\\\\\\ */
int		wildcard(t_dll **tokens, t_dll *curr);
int		hidden_files(char *val, char *name);
int		search_for_match(t_dll **tokens, t_dll *curr, char *val, char *d_name);
int 	prefix(char *name, char *val, int *start);
int		infix(char *val, char *name, int i);
int		last_infix_check(char *val, char *name, int i, int j);
int 	suffix(char *name, char *val);
/* ///////////////// heredoc \\\\\\\\\\\\\\\\\\\\\ */
void	heredoc(t_dll **tokens, t_env *env, int e_code);
void	handle_herdoc(t_dll *_Next, char *name, t_env *env, int e_code);
void	expandable_doc(char *delim, char *name, t_env *env, int e_code);
char	*expand_in_heredoc(char *value, t_env *env, int e_code);
void	non_expandable_doc(char *delim, char *name);
void	replace_tokens(t_dll **tokens, t_dll *curr, t_dll *_Next, char *name);
char	*ft_strnstr(const char *big, const char *little, int len);
void	last_check_doc(t_dll **tokens);
/* ///////////////// redirections \\\\\\\\\\\\\\\\\\\\\ */
void	redirect(t_dll **tokens);
int		handle_redirect(char *value, t_dll *_Next);
int		redirections(t_dll **tokens);
void    restore_fds(t_dll *token);
void    identify_redirections(t_dll **tokens);
int		in_fd(t_dll *token);
int		out_fd(t_dll *token, int O_FLAG);
// ------------------------------------------------------------------ //

#endif