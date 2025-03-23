
#ifndef SMASH_H
# define SMASH_H

# include <stdio.h> 
# include <stdlib.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../execution/execution.h"

#define RESET   "\033[0m"       // Reset all attributes
#define RED     "\033[31m"      // Red text

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
	// Token data
	char			*value;
    t_token_type 	token_type;
	t_quote_type	quote_type;
	int				expandable;
	int				direction;
	//Token parsing & execution
	int			precedence;
	int			exec_order;
	// Token positioning
	struct s_dll	*prev;
	struct s_dll	*next;
}  	t_dll;
// -------------------- Everything about tokens -------------------- //
/* ///////////////// Basic token operation \\\\\\\\\\\\\\\\\\\\\ */ 
t_dll	*create_token_list(void);
void	add_token(t_dll **head, char *val, t_token_type token_type);
void	remove_token(t_dll **head, t_dll *token);
void	free_token_list(t_dll **head);
/* ///////////////// token navigation \\\\\\\\\\\\\\\\\\\\\ */
t_dll	*get_next_token(t_dll *current_token);
t_dll	*get_previous_token(t_dll *current_token);
t_dll	*find_token(t_dll *head, t_token_type token_type);
/* ///////////////// token & parsing modification \\\\\\\\\\\\\\\\\\\ */
char	*ft_merge(char *first_val, char *second_val);
void	merge_tokens(t_dll *first_token, t_dll *second_token);
t_dll	*tokenize_input(char *input);
/* ///////////////// helpers \\\\\\\\\\\\\\\\\\\\\\\\\\\ */ 
t_token_type	get_token_type(char *val);
t_quote_type	get_quote_type(char *val);
char			*get_val(char *input);
int				ft_strncmp(char *s1, char *s2, int n);
char			*ft_strduplen(char *input, int len);
char			*ft_strchr(const char *s, int c);
int				ft_strlen(const char *s);
void			Error(char *val, t_error_type error);
// ----------------------------------------------------------------- //
// -------------------- Everything about Parsing -------------------- //
int		parse_input(t_dll *tokens);
void	check_for_merge(t_dll *tokens);
int		check_quotes(t_dll *tokens);
void	merge_quotes(t_dll *tokens);
// ------------------------------------------------------------------ //

#endif