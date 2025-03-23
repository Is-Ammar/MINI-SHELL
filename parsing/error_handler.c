# include "smash.h"

void	Error(char *val, t_error_type error)
{
    if (error == EQUOTES)
        printf("minishell: unexpected EOF while looking for matching `%c'\n", *val);
    else if (error == EBRACKET)
        printf("minishell: unexpected EOF while looking for matching `)'\n");
    else if (error == ESYNTAX)
        printf("minishell: syntax error near unexpected token `%s'\n", val);
}