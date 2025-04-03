/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_modifiers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/03 14:21:55 by habdella         ###   ########.fr       */
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
				if (!ft_strcmp(curr->value, "<<"))
					curr->heredoc = TRUE;
			}
		}
		curr->quote_type = get_quote_type(curr->value);
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
			if (ft_strchr(curr->value, '$') && curr->quote_type != SQUOTE)
				curr->expandable = TRUE;
			continue ;
		}
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
		if (ft_strchr(curr->value, '$'))
			curr->expandable = TRUE;
		if (curr->value[0] == '<')
			curr->direction = LEFT;
		else if (curr->value[0] == '>')
			curr->direction = RIGHT;
		if (curr->token_type == WHITESPACE)
			remove_token(tokens, curr);
		curr = curr->next;
	}	
}

void	expand_vars(t_dll *tokens, t_env *env)
{
	t_dll	*curr;
	char	*temp;

	if (!tokens)
		return ;
	curr = tokens;
	temp = NULL;
	while (curr)
	{
		if (curr->expandable == TRUE)
		{
			temp = curr->value;
			curr->value = expanding(curr->value, env);
			if (temp)
				free(temp);
		}
		curr = curr->next;
	}
}

void	remove_quotes_expand(t_dll **tokens, t_env **env)
{
	t_dll	*curr;
	char	*temp;

	if (!tokens || !*tokens)
		return ;
	expand_vars(*tokens, *env);
	curr = *tokens;
	temp = NULL;
	while (curr && curr->token_type != OPERATOR)
	{
		if (ft_strchr(curr->value, '\'') || ft_strchr(curr->value, '"'))
		{
			temp = curr->value;
			curr->value = ft_strdup_quotes(curr->value);
			free(temp);
			curr->quote_type = NONE;
		}
		curr = curr->next;
	}
}
