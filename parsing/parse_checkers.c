/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_checkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/31 14:14:50 by habdella         ###   ########.fr       */
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
		if (curr->quote_type == SQUOTE)
		{
			while (curr->value[i])
				i++;
			if (curr->value[--i] != '\'' || ft_strlen(curr->value) == 1)
				return (Error(curr->value, EQUOTES), 1);
		}
		else if (curr->quote_type == DQUOTE)
		{
			while (curr->value[i])
				i++;
			if (curr->value[--i] != '"' || ft_strlen(curr->value) == 1)
				return (Error(curr->value, EQUOTES), 1);
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
		return (Error(tokens->value, EBRACKET), 1);
	return (0);
}

int	additional_check_logic(t_dll *curr)
{
	if (curr->token_type == REDIRECTION)
		return (Error("newline", ESYNTAX), 1);
	if (curr->token_type != SYMBOL && curr->token_type != WORD)
		return (Error(curr->value, ESYNTAX), 1);
	return (0);
}

int		check_logic(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return (0);
	(1) && (curr = tokens, Next = curr->next);
	if (curr->token_type != WORD && curr->token_type != REDIRECTION 
		&& curr->token_type != SYMBOL)
		return (Error(curr->value, ESYNTAX), 1);
	if (curr->direction == LEFT && Next && Next->direction == RIGHT)
		return (Error("newline", ESYNTAX), 1);
	if (curr->token_type == REDIRECTION && !Next)
		return (Error("newline", ESYNTAX), 1);
	while (curr && curr->next)
	{
		Next = curr->next;
		if (curr->token_type == REDIRECTION 
			&& Next->token_type != WORD && Next->token_type != SYMBOL)
			return (Error(Next->value, ESYNTAX), 1);
		if (curr->token_type != WORD && curr->token_type != SYMBOL && Next->token_type != WORD
			&& Next->token_type != SYMBOL && Next->token_type != REDIRECTION)
			return (Error(Next->value, ESYNTAX), 1);
		curr = curr->next;
	}
	return (additional_check_logic(curr));
}

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

int	parse_input(t_dll **tokens)
{
	operators_merge(tokens);
	if (check_quotes(*tokens))
		return (1);
	merge_quotes(tokens);
	if (check_brackets(*tokens))
		return (1);
	remove_spaces(tokens);
	if (check_logic(*tokens))
		return (1);
	if (find_token(*tokens, SYMBOL) != NULL)
	{
		if (check_subshell(*tokens))
			return (1);
	}
	return (0);
}
