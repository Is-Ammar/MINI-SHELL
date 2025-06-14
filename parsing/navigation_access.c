/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation_access.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/14 16:03:03 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*ft_merge(t_shell *shell, char *first_val, char *second_val)
{
	char	*merge;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(first_val) + ft_strlen(second_val);
	merge = ft_malloc(shell, len + 1, 0);
	if (!merge)
		return (NULL);
	i = 0;
	while (first_val[i])
	{
		merge[i] = first_val[i];
		i++;
	}
	j = 0;
	while (second_val[j])
	{
		merge[i++] = second_val[j++];
	}
	merge[i] = '\0';
	return (merge);
}

void	merge_tokens(t_shell *shell, t_dll *first_token, t_dll *second_token)
{
	char	*temp;

	if (!first_token || !second_token)
		return ;
	temp = first_token->value;
	first_token->value = ft_merge(shell, temp, second_token->value);
	if (second_token->prev)
		second_token->prev->next = first_token;
	if (second_token->next)
		second_token->next->prev = first_token;
	first_token->next = second_token->next;
}

t_dll	*find_token(t_dll *head, t_token_type token_type)
{
	t_dll	*curr;

	if (!head)
		return (NULL);
	curr = head;
	while (curr)
	{
		if (curr->token_type == token_type)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

t_dll	*find_command(t_shell *shell, t_dll *head)
{
	t_dll	*curr;
	char	*tmp;

	if (!head)
		return (NULL);
	curr = head;
	tmp = NULL;
	while (curr)
	{
		if (curr->token_type == WORD)
		{
			tmp = expand_env_str(shell, curr->value);
			if (tmp)
				return (curr);
		}
		curr = curr->next;
	}
	head->fake_cmd = TRUE;
	return (head);
}
