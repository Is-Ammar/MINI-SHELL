/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/23 18:03:51 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	remove_spaces(t_dll *tokens)
{
	t_dll	*curr;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr)
	{
		if (curr->value[0] == '<')
			curr->direction = LEFT;
		else if (curr->value[0] == '>')
			curr->direction = RIGHT;
		if (curr->token_type == WHITESPACE)
			remove_token(&tokens, curr);
		curr = curr->next;
	}	
}

int		check_logic(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return (1);
	(1) && (curr = tokens, Next = curr->next);
	if (curr->token_type != WORD && curr->token_type != REDIRECTION)
		return (Error(curr->value, ESYNTAX), 1);
	if (curr->direction == LEFT && Next && Next->direction == RIGHT)
		return (Error("newline", ESYNTAX), 1);
	if (curr->token_type == REDIRECTION && !Next)
		return (Error("newline", ESYNTAX), 1);
	while (curr && curr->next)
	{
		Next = curr->next;
		if (curr->token_type == REDIRECTION && Next->token_type != WORD)
			return (Error(Next->value, ESYNTAX), 1);
		if (curr->token_type != WORD
			&&(Next->token_type != WORD && Next->token_type != REDIRECTION))
			return (Error(Next->value, ESYNTAX), 1);
		curr = curr->next;
	}
	if (curr->token_type == REDIRECTION)
		return (Error("newline", ESYNTAX), 1);
	return (0);
}