/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_modifiers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/30 08:11:55 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	operators_merge(t_shell *shell, t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr && curr->value && nxt && nxt->value
			&& !ft_strcmp(curr->value, nxt->value))
		{
			if (!ft_strcmp(curr->value, "&") || !ft_strcmp(curr->value, "|")
				|| !ft_strcmp(curr->value, "<") || !ft_strcmp(curr->value, ">"))
			{
				merge_tokens(shell, curr, nxt);
				curr->token_type = get_token_type(curr->value);
				continue ;
			}
		}
		curr = nxt;
	}
	if (curr)
		curr->quote_type = get_quote_type(curr->value);
}

void	merge_quotes(t_shell *shell, t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr->token_type == WORD && nxt->token_type == WORD)
		{
			merge_tokens(shell, curr, nxt);
			curr->token_type = get_token_type(curr->value);
			curr->quote_type = get_quote_type(curr->value);
			continue ;
		}
		curr->quote_type = get_quote_type(curr->value);
		curr = nxt;
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
		if (check_depth_to_expand(curr->value))
			curr->wildcard = TRUE;
		if (ft_strchr(curr->value, '$'))
			curr->expandable = TRUE;
		if (!ft_strcmp(curr->value, "||"))
			curr->operator = OR;
		if (!ft_strcmp(curr->value, "&&"))
			curr->operator = AND;
		if (!ft_strcmp(curr->value, "<<"))
			curr->heredoc = TRUE;
		if (curr->value && curr->value[0] == '<')
			(curr->direction = LEFT);
		if (curr->value && curr->value[0] == '>')
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

int	expansion(t_shell *shell, t_dll **tokens, t_dll **token)
{
	int		ambiguous;
	t_dll	*curr;

	if (!tokens || !*tokens)
		return (0);
	ambiguous = 0;
	curr = *token;
	if (curr->expandable == TRUE || curr->quote_type != NONE)
	{
		if (expand_execute(shell, tokens, curr))
			return (1);
	}
	if (curr->wildcard == TRUE)
	{
		ambiguous = wildcard(shell, tokens, curr);
		curr->wildcard = FALSE;
		if (curr->token_type == REDIRECTION && ambiguous > 1)
		{
			parse_error(curr->value, EAMBIGUO);
			return (1);
		}
		if (curr->token_type == REDIRECTION && (*token)->next)
			(*token) = (*token)->next;
	}
	return (0);
}
