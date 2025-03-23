/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/23 01:10:59 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smash.h"

void	check_for_merge(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr && curr->next)
	{
		Next = curr->next;
		if (Next && !ft_strncmp(curr->value, Next->value, 1))
		{
			if (curr->value[0] == '&' || curr->value[0] == '|'
				|| curr->value[0] == '<' || curr->value[0] == '>')
			{
				merge_tokens(curr, Next);
				curr->token_type = get_token_type(curr->value);
			}
		}
		curr->quote_type = get_quote_type(curr->value);
		curr = curr->next;
	}
	if (curr)
		curr->quote_type = get_quote_type(curr->value);
}

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

void	merge_quotes(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr && curr->next)
	{
		Next = curr->next;
		if ((curr->token_type == WORD && Next->token_type == WORD)
			|| (curr->quote_type != NONE && Next->quote_type != NONE))
		{
			merge_tokens(curr, Next);
			curr->token_type = get_token_type(curr->value);
			continue ;
		}
		curr = curr->next;
	}
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
			count++;
		else if (curr->value[0] == ')')
			count--;
		curr = curr->next;
	}
	if (count)
		return (Error(tokens->value, EBRACKET), 1);
	return (0);
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

int	parse_input(t_dll *tokens)
{
	check_for_merge(tokens);
	if (check_quotes(tokens))
		return (1);
	merge_quotes(tokens);
	if (check_brackets(tokens))
		return (1);
	remove_spaces(tokens);
	if (check_logic(tokens))
		return (1);
	return (0);
}
