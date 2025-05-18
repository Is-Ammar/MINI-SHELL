/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hered_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/18 10:21:45 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

char	*ft_strnstr(const char *big, const char *little, int len)
{
	int	i;
	int	j;

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

char	*expand_in_heredoc(t_shell *shell, char *value)
{
	char	*new_val;
	int		i;
	int		j;

	i = 0;
	new_val = ft_strdup(shell, "");
	while (value[i])
	{
		j = i;
		while (value[i] && value[i] != '$')
			i++;
		if (j != i)
			new_val = ft_strjoin(shell, new_val, ft_strduplen(shell, &value[j] \
			, i - j));
		else if (value[i] == '$')
			new_val = ft_strjoin(shell, new_val, dollar_sign(shell, value, &i \
			, FALSE));
	}
	return (new_val);
}

void	last_check_doc(t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (ft_strnstr(curr->value, "/tmp/.heredoc_", 14)
			&& ft_strnstr(nxt->value, "/tmp/.heredoc_", 14))
		{
			if (!ft_strcmp(curr->value, nxt->value))
			{
				remove_token(tokens, nxt);
				continue ;
			}
		}
		curr = nxt;
	}
}
