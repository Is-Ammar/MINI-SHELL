/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_modifiers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/05 15:28:26 by iammar           ###   ########.fr       */
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
		if (nxt && curr->value[0] == nxt->value[0])
		{
			if (curr->value[0] == '&' || curr->value[0] == '|'
				|| curr->value[0] == '<' || curr->value[0] == '>')
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

int	expansion(t_shell *shell, t_dll **tokens, t_dll *curr)
{
	// t_env	*env; unused ---
	// int		e_code; unused ---

	if (!tokens || !*tokens)
		return (0);
	// env = shell->env_list;
	// e_code = shell->exit_code;
	if (curr->expandable == TRUE || curr->quote_type != NONE)
	{
		curr->value = expand_env_vars(shell, curr->value);
	}
	if (curr->wildcard == TRUE)
		wildcard(shell, tokens, curr);
	return (0);
}
