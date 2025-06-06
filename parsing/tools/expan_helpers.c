/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/06 15:50:53 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	check_spaces(char *val)
{
	int	i;

	if (!val)
		return (0);
	if (!*val)
		return (1);
	i = 0;
	while (val[i] && ft_strchr(WHITESPACES, val[i]))
		i++;
	if (val[i] == '\0')
		return (0);
	return (1);
}

int	is_removable(char *val)
{
	int	i;

	i = 0;
	while (val[i])
	{
		if (val[i] == '\'' && val[i + 1] && val[i + 1] == '\'')
			return (1);
		if (val[i] == '"' && val[i + 1] && val[i + 1] == '"')
			return (1);
		i++;
	}
	return (0);
}

t_dll	*add_to_tokens(t_shell *shell, t_dll **head, t_dll *token, char *val)
{
	t_dll	*nxt;
	t_dll	*new_token;

	new_token = create_token_list(shell);
	new_token->value = ft_strdup(shell, val);
	new_token->token_type = WORD;
	identify_tokens(new_token);
	if (!*head)
	{
		*head = new_token;
		return (new_token);
	}
	if (!token)
		return (*head);
	nxt = token->next;
	token->next = new_token;
	new_token->prev = token;
	new_token->next = nxt;
	if (nxt)
		nxt->prev = new_token;
	return (new_token);
}

t_dll	*split_tok(t_shell *shell, t_dll **tokens, t_dll *curr, t_expand *exp)
{
	t_dll	*token;
	char	*token_val;
	int		i;
	int		start;

	(1) && (i = 0, token = curr);
	while (exp->str && exp->str[i])
	{
		while (exp->str[i] && ft_strchr(WS, exp->str[i]) && exp->mask[i] == '1')
			i++;
		start = i;
		while (exp->str[i])
		{
			if (ft_strchr(WHITESPACES, exp->str[i]) && exp->mask[i] == '1')
				break ;
			i++;
		}
		token_val = ft_strduplen(shell, &exp->str[start], i - start);
		if (!token_val)
			continue ;
		token = add_to_tokens(shell, tokens, token, token_val);
		token->is_splited = TRUE;
		token->quote_type = get_quote_type(token_val);
	}
	return (token);
}
