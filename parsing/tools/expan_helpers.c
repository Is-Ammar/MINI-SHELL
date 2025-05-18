/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/18 15:06:30 by habdella         ###   ########.fr       */
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

void	merge_operators(t_shell *shell, t_dll **tokens, int count)
{
	t_dll	*curr;
	t_dll	*nxt;
	int		i;

	if (!tokens || !*tokens)
		return ;
	i = 0;
	curr = *tokens;
	while (curr && curr->next && i < count)
	{
		nxt = curr->next;
		if (nxt && curr && curr->value && nxt->value
			&& curr->value[0] == nxt->value[0])
		{
			if (curr->value[0] == '&' || curr->value[0] == '|'
				|| curr->value[0] == '<' || curr->value[0] == '>')
			{
				merge_tokens(shell, curr, nxt);
			}
				
		}
		curr->token_type = WORD;
		curr = nxt;
	}
}

// void	remove_space(t_dll **tokens, int count)
// {
// 	t_dll	*curr;

// 	if (!tokens || !*tokens)
// 		return ;
// 	curr = *tokens;
// 	while (curr)
// 	{
// 		if (curr->token_type == WHITESPACE)
// 			remove_token(tokens, curr);
// 		curr = curr->next;
// 	}
// }

int	should_be_splited(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '\'')
		{
			i++;
			while (value[i] && value[i] != '\'')
				i++;
		}
		if (value[i] == '"')
		{
			i++;
			while (value[i] && value[i] != '"')
				i++;
		}
		if (value[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

void	split_token(t_shell *shell, t_dll **tokens, t_dll *curr, char *input)
{
	t_dll	*token;
	char	*token_val;
	int		index;
	// int		count;

	index = 0;
	// count = 0;
	token = curr;
	while (input[index])
	{
		while (input[index] && input[index] == ' ')
			index++;
		token_val = get_token_val(shell, input, &index);
		if (!token_val)
			return ;
		token = add_to_tokens(shell, tokens, token, token_val);
		// count++;
	}
	remove_token(tokens, curr);
	// merge_operators(shell, tokens, count);
	// remove_spaces(tokens);
}
