/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_modifiers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/16 10:37:14 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	operators_merge(t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		Next = curr->next;
		if (Next && curr->value[0] == Next->value[0])
		{
			if (curr->value[0] == '&' || curr->value[0] == '|'
				|| curr->value[0] == '<' || curr->value[0] == '>')
			{
				merge_tokens(curr, Next);
				curr->token_type = get_token_type(curr->value);
			}
		}
		curr = curr->next;
	}
	if (curr)
		curr->quote_type = get_quote_type(curr->value);
}

void	merge_quotes(t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		Next = curr->next;
		if (curr->token_type == WORD && Next->token_type == WORD)
		{
			merge_tokens(curr, Next);
			curr->token_type = get_token_type(curr->value);
			curr->quote_type = get_quote_type(curr->value);
			continue ;
		}
		curr->quote_type = get_quote_type(curr->value);
		curr = Next;
	}
}

void	identify_tokens(t_dll *tokens)
{
	t_dll	*curr;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr)
	{
		if (!check_depth_to_expand(curr->value))
			curr->wildcard = TRUE;
		if (ft_strchr(curr->value, '$'))
			curr->expandable = TRUE;
		if (!ft_strcmp(curr->value, "||"))
			curr->operator = OR;
		if (!ft_strcmp(curr->value, "&&"))
			curr->operator = AND;
		if (!ft_strcmp(curr->value, "<<"))
			curr->heredoc = TRUE;
		if (curr->value[0] == '<')
			(curr->direction = LEFT);
		if (curr->value[0] == '>')
			(curr->direction = RIGHT);
		curr = curr->next;
	}
}

void	remove_spaces(t_dll **tokens)
{
	t_dll	*curr;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr)
	{
		if (curr->token_type == WHITESPACE)
			remove_token(tokens, curr);
		curr = curr->next;
	}
}

int	expansion(t_dll **tokens, t_env *env, int e_code)
{
	t_dll	*curr;
	char	*temp;

	if (!tokens || !*tokens)
		return (0);
	curr = *tokens;
	temp = NULL;
	while (curr /*&& curr->token_type != OPERATOR*/)
	{
		if (curr->expandable == TRUE || curr->quote_type != NONE)
		{
			temp = curr->value;
			curr->value = expand_env_vars(curr->value, env, e_code);
			if (temp)
				free(temp);
		}
		if (curr->wildcard == TRUE)
			if (wildcard(tokens, curr))
				return (1);
		curr = curr->next;
	}
	return (0);
}
