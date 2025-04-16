/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hered_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/15 12:31:09 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

char	*ft_strnstr(const char *big, const char *little, int len)
{
	int i;
	int j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (i < len && big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && (i + j) < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)(big + i));
			j++;
		}
		i++;
	}
	return (NULL);
}

void	last_check_doc(t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*_Next;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		_Next = curr->next;
        if (ft_strnstr(curr->value, "/tmp/.heredoc_", 14) 
            && ft_strnstr(_Next->value, "/tmp/.heredoc_", 14))
		{
			if (!ft_strcmp(curr->value, _Next->value))
			{
				remove_token(tokens, _Next);
				continue ;
			}
		}
		curr = _Next;
	}
}

void	replace_tokens(t_dll **tokens, t_dll *curr, t_dll *_Next, char *name)
{
	remove_token(tokens, _Next);
	add_mid_token(tokens, curr, name);
	remove_token(tokens, curr);
}