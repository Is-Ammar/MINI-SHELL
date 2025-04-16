/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/15 12:21:24 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_subshell(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return (1);
	(1) && (curr = tokens, Next = curr->next);
	while (curr && curr->next)
	{
		Next = curr->next;
		if (curr->token_type == REDIRECTION && Next->bracket == RIGHT)
			return (Error(Next->value, ESYNTAX), 1);
		if (curr->bracket == RIGHT && Next->bracket == LEFT)
			return (Error(Next->value, EBRACKET), 1);
		if (curr->bracket == RIGHT && Next->token_type != WORD
			&& Next->token_type != REDIRECTION && Next->token_type != SYMBOL)
			return (Error(Next->value, ESYNTAX), 1);
		if (curr->bracket == LEFT && Next->token_type == WORD)
			return (Error(Next->value, ESYNTAX), 1);
		if (curr->token_type != WORD && curr->token_type != SYMBOL && Next->bracket == LEFT)
			return (Error(Next->value, ESYNTAX), 1);
		if (curr->token_type == REDIRECTION && Next->bracket == LEFT)
			return (Error(Next->value, EBRACKET), 1);
		curr = curr->next;
	}
	return (0);
}

int	subshell_last(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*_Next;
	int		flag;

	if (!tokens)
		return (0);
	curr = tokens;
	flag = 0;
	while (curr && curr->next)
	{
		_Next = curr->next;
		if (curr->bracket == LEFT && _Next->token_type == REDIRECTION)
			flag = 1;
		if (flag && _Next->token_type == WORD && _Next->next
			&& _Next->next->token_type == WORD)
			return (Error(_Next->next->value, ESYNTAX), 1);
		curr = _Next;
	}
	return (0);
}
