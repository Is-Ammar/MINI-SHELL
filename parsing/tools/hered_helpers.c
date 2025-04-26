/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hered_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/25 11:27:47 by habdella         ###   ########.fr       */
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

char	*expand_in_heredoc(char *value, t_env *env, int e_code)
{
	char	*new_val;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	new_val = ft_strdup("");
	while (value[i])
	{
		j = i;
		while (value[i] && value[i] != '$')
			i++;
		temp = new_val;
		if (j != i)
			new_val = ft_strjoin(new_val, ft_strduplen(&value[j], i - j));
		else if (value[i] == '$')
			new_val = ft_strjoin(new_val, dollar_sign(value, &i, env, e_code));
		if (temp)
			free(temp);
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

void	replace_tokens(t_dll **tokens, t_dll *curr, t_dll *nxt, char *name)
{
	remove_token(tokens, nxt);
	add_mid_token(tokens, curr, name);
	remove_token(tokens, curr);
}
