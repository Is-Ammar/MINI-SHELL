/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation_access.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/24 21:40:55 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_dll	*get_next_token(t_dll *current_token)
{
	if (!current_token || !current_token->next)
		return (NULL);
	return (current_token->next);
}

t_dll	*get_previous_token(t_dll *current_token)
{
	if (!current_token || !current_token->prev)
		return (NULL);
	return (current_token->prev);
}

char	*ft_merge(char *first_val, char *second_val)
{
	char	*merge;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(first_val) + ft_strlen(second_val);
	merge = malloc(len + 1);
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

void	merge_tokens(t_dll *first_token, t_dll *second_token)
{
	char	*temp;

	if (!first_token || !second_token)
		return ;
	
	temp = first_token->value;
	first_token->value = ft_merge(temp, second_token->value);
	free(temp);
	
	if (second_token->prev)
		second_token->prev->next = first_token;
	if (second_token->next)
		second_token->next->prev = first_token;
	first_token->next = second_token->next;
	
	free(second_token->value);
	free(second_token);
}

t_dll	*find_token(t_dll *head, t_token_type token_type)
{
	if (!head)
		return (NULL);
	while (head)
	{
		if (head->token_type == token_type)
			return (head);
		head = head->next;
	}
	return (NULL);
}
