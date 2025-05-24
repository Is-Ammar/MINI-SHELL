/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/19 08:06:45 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_strlen_spaces(char *val, int *index)
{
	int	i;
	int	len;
	int	flag;

	(1) && (i = 0, len = 0, *index = ft_strlen(val) - 1);
	while (val[i] && val[i] == ' ')
		i++;
	while (i < *index && val[*index] == ' ')
		(*index)--;
	while (i <= *index)
	{
		flag = 0;
		while (val[i] == ' ')
		{
			if (!flag)
				len++;
			flag = 1;
			i++;
		}
		len++;
		i++;
	}
	return (len);
}

char	*remove_too_much_spaces(t_shell *shell, char *val)
{
	int		(i), (j);
	int		len;
	int		flag;
	char	*p;

	(1) && (i = 0, j = 0);
	p = ft_malloc(shell, ft_strlen_spaces(val, &len) + 1);
	while (val[i] && val[i] == ' ')
		i++;
	while (i <= len)
	{
		flag = 0;
		while (val[i] == ' ')
		{
			if (!flag)
				p[j++] = val[i];
			flag = 1;
			i++;
		}
		p[j++] = val[i];
		i++;
	}
	p[j] = '\0';
	return (p);
}

int	is_ambiguous(char *val)
{
	int		i;
	int		j;

	if (!*val)
		return (1);
	i = 0;
	j = ft_strlen(val) - 1;
	while (val[i] && val[i] == ' ')
		i++;
	while (i < j && val[j] == ' ')
		j--;
	while (i <= j)
	{
		if (val[i] == ' ')
			return (1);
		i++;
	}
	return (0);
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

int expand_execute(t_shell *shell, t_dll **tokens, t_dll *curr)
{
    char	*tmp;

    tmp = curr->value;
    curr->value = expand_env_vars(shell, curr->value);
	if (curr->token_type == REDIRECTION && ft_strchr(tmp, '$')
        && curr->value[0] == '\0')
	{
		parse_error(tmp, EAMBIGUO);
		remove_token(tokens, curr);
		return (1);
	}
	if (curr->quote_type != DQUOTE && curr->expandable)
	{
		curr->value = remove_too_much_spaces(shell, curr->value);
		if (should_be_splited(curr->value))
			split_token(shell, tokens, curr, curr->value);
	}
	if (curr->token_type == REDIRECTION && curr->expandable
		&& curr->quote_type == NONE && is_ambiguous(curr->value))
		return (parse_error(tmp, EAMBIGUO), 1);
	if (!is_removable(tmp) && curr->value[0] == '\0' && curr->expandable)
		remove_token(tokens, curr);
	curr->expandable = FALSE;
    return (0);
}

