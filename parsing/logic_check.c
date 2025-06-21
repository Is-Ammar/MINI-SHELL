/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/21 17:59:36 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	additional_check_logic(t_dll *curr)
{
	if (curr->token_type == REDIRECTION)
		return (parse_error("newline", ESYNTAX), 1);
	if (curr->token_type != SYMBOL && curr->token_type != WORD)
		return (parse_error(curr->value, ESYNTAX), 1);
	return (0);
}

int	check_subshell(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	(1) && (curr = tokens, nxt = curr->next);
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr->token_type == REDIRECTION && nxt->bracket == RIGHT)
			return (parse_error(nxt->value, ESYNTAX), 1);
		if (curr->bracket == RIGHT && nxt->bracket == LEFT)
			return (parse_error(nxt->value, EBRACKET), 1);
		if (curr->bracket == RIGHT && nxt->token_type != WORD
			&& nxt->token_type != REDIRECTION && nxt->token_type != SYMBOL)
			return (parse_error(nxt->value, ESYNTAX), 1);
		if (curr->bracket == LEFT && nxt->token_type == WORD)
			return (parse_error(nxt->value, ESYNTAX), 1);
		if (curr->token_type != WORD && curr->token_type != SYMBOL
			&& nxt->bracket == LEFT)
			return (parse_error(nxt->value, ESYNTAX), 1);
		if (curr->token_type == REDIRECTION && nxt->bracket == LEFT)
			return (parse_error(nxt->value, EBRACKET), 1);
		curr = nxt;
	}
	return (0);
}

int	subshell_redirection(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	curr = tokens;
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr->bracket == LEFT && nxt->token_type == REDIRECTION)
		{
			curr = nxt;
			if (curr->next && curr->next->next)
			{
				curr = curr->next;
				nxt = curr->next;
				if (curr->token_type == WORD && nxt->token_type == WORD)
					return (parse_error(nxt->value, ESYNTAX), 1);
			}
		}
		if (curr)
			curr = nxt;
	}
	return (0);
}

int	subshell_last(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	if (!tokens)
		return (0);
	curr = tokens;
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr->bracket == LEFT && nxt->bracket == RIGHT)
			return (parse_error(nxt->value, ESYNTAX), 1);
		if (curr->token_type == WORD && nxt->bracket == RIGHT)
			return (parse_error(nxt->next->value, ESYNTAX), 1);
		curr = nxt;
	}
	return (subshell_redirection(tokens));
}
