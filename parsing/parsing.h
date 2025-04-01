/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/01 15:00:40 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h> 
# include <stdlib.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../execution/execution.h"

#define RESET   "\001\033[0m\002"		// Reset all attributes
#define RED     "\033[31m"				// Red text
#define CYAN	"\001\033[0;36m\002"	// Cyan text

# define FALSE	0
# define TRUE	1
# define METACHARS	"( )\t<|&>'\"$"
# define LEFT	1
# define RIGHT	2

typedef	enum e_error_type
{
	EQUOTES,
	EBRACKET,
	ESYNTAX,
	ECOMMAND
}	t_error_type;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECTION,
	OPERATOR,
	SYMBOL,
	WHITESPACE,
	INVALID
}	t_token_type;

typedef enum e_quote_type
{
	NONE,
	SQUOTE,
	DQUOTE
}	t_quote_type;

typedef struct s_dll
{
	char			*value;
	t_token_type 	token_type;
	t_quote_type	quote_type;
	int				expandable;
	int				direction;
	int				bracket;
	int				exec_order;
	struct s_dll	*prev;
	struct s_dll	*next;
}  	t_dll;

typedef struct s_shell	t_shell;
typedef	struct s_env	t_env;

// -------------------- Everything about tokens -------------------- //

/* ///////////////// Basic token operation \\\\\\\\\\\\\\\\\\\\\ */ 
t_dll	*create_token_list(void);
void	add_token(t_dll **head, char *val, t_token_type token_type);
void	remove_token(t_dll **head, t_dll *token);
void	free_token_list(t_dll **head);
/* ///////////////// token navigation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*find_token(t_dll *head, t_token_type token_type);
/* ///////////////// token & parsing modification \\\\\\\\\\\\\\\\\\\ */
char	*ft_merge(char *first_val, char *second_val);
void	merge_tokens(t_dll *first_token, t_dll *second_token);
t_dll	*tokenize_input(char *input);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */ 
t_token_type	get_token_type(char *val);
t_quote_type	get_quote_type(char *val);
char			*get_token_val(char *input);
int				ft_strlen(const char *s);
int				ft_strlen_quotes(const char *s);
char			*ft_strduplen(char *input, int len);
char			*ft_strdup_quotes(char	*token);
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
int		check_logic(t_dll *tokens);
/* ///////////////// modifiers \\\\\\\\\\\\\\\\\\\\\ */ 
void	operators_merge(t_dll **tokens);
void	merge_quotes(t_dll **tokens);
void	remove_spaces(t_dll **tokens);
char	*expand(char *value, t_env *env);
void	expand_vars(t_dll *token, t_env *env);
void	remove_quotes_expand(t_dll **tokens, t_env **env);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */ 
int		ft_stristr(char *big, char *little);
char	*remove_str(char *token, char *remove);
char	*ft_strdup_expand(char *token, t_env *env, int start, int end);
char	*ft_strjoin(const char *s1, const char *s2);
// ------------------------------------------------------------------ //

#endif