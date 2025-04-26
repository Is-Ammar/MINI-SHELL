/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_checkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/25 11:41:16 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_quotes(t_dll *tokens)
{
	t_dll	*curr;
	int		i;

	curr = tokens;
	while (curr)
	{
		i = 0;
		if (curr->value[i] == '\'')
		{
			while (curr->value[i])
				i++;
			if (curr->value[--i] != '\'' || ft_strlen(curr->value) == 1)
				return (ft_error(curr->value, EQUOTES), 1);
		}
		else if (curr->value[i] == '"')
		{
			while (curr->value[i])
				i++;
			if (curr->value[--i] != '"' || ft_strlen(curr->value) == 1)
				return (ft_error(curr->value, EQUOTES), 1);
		}
		curr = curr->next;
	}
	return (0);
}

int	check_brackets(t_dll *tokens)
{
	t_dll	*curr;
	int		count;

	curr = tokens;
	count = 0;
	while (curr)
	{
		if (count < 0)
			break ;
		if (curr->value[0] == '(')
		{
			curr->bracket = RIGHT;
			count++;
		}
		else if (curr->value[0] == ')')
		{
			curr->bracket = LEFT;
			count--;
		}
		curr = curr->next;
	}
	if (count)
		return (ft_error(tokens->value, EBRACKET), 1);
	return (0);
}

int	additional_check_logic(t_dll *curr)
{
	if (curr->token_type == REDIRECTION)
		return (ft_error("newline", ESYNTAX), 1);
	if (curr->token_type != SYMBOL && curr->token_type != WORD)
		return (ft_error(curr->value, ESYNTAX), 1);
	return (0);
}

int	check_logic(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	if (!tokens)
		return (0);
	(1) && (curr = tokens, nxt = curr->next);
	if (curr->token_type != WORD && curr->token_type != REDIRECTION
		&& curr->token_type != SYMBOL)
		return (ft_error(curr->value, ESYNTAX), 1);
	if (curr->token_type == REDIRECTION && !nxt)
		return (ft_error("newline", ESYNTAX), 1);
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr->token_type == REDIRECTION
			&& nxt->token_type != WORD && nxt->token_type != SYMBOL)
			return (ft_error(nxt->value, ESYNTAX), 1);
		if (curr->token_type != WORD && curr->token_type != SYMBOL
			&& nxt->token_type != WORD && nxt->token_type != SYMBOL
			&& nxt->token_type != REDIRECTION)
			return (ft_error(nxt->value, ESYNTAX), 1);
		curr = nxt;
	}
	return (additional_check_logic(curr));
}

int	parse_input(t_dll **tokens)
{
	operators_merge(tokens);
	if (check_quotes(*tokens))
		return (1);
	merge_quotes(tokens);
	if (check_brackets(*tokens))
		return (1);
	remove_spaces(tokens);
	identify_tokens(*tokens);
	if (check_logic(*tokens))
		return (1);
	if (find_token(*tokens, SYMBOL) != NULL)
	{
		if (check_subshell(*tokens) || subshell_last(*tokens))
			return (1);
	}
	return (0);
}
