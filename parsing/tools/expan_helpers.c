/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/29 08:27:08 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

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

void	split_token(t_shell *shell, t_dll **tokens, t_dll *curr, char *input, char *mask)
{
	t_dll	*token;
	char	*token_val;
	int		i;
	int		start;

	i = 0;
	token = curr;
	while (input[i])
	{
		while (input[i] && input[i] == ' ' && mask[i] == '1')
			i++;
		start = i;
		while (input[i])
		{
			if (input[i] == ' ' && mask[i] == '1')
				break;
			i++;
		}
		token_val = ft_strduplen(shell, &input[start], i - start);
		if (!token_val)
			continue ;
		token = add_to_tokens(shell, tokens, token, token_val);
		token->quote_type = get_quote_type(token_val);
	}
	remove_token(tokens, curr);
}
