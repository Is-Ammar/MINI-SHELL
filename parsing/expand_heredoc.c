/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 11:26:40 by habdella          #+#    #+#             */
/*   Updated: 2025/06/08 14:42:05 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*delim_dollar(t_shell *shell, char *value, int *i, int is_dquote)
{
	int		len;
	int		flag;
	int		start;
	char	*p;

	p = NULL;
	flag = 0;
	start = *i + 1;
	len = start;
	if (is_dquote == TRUE && (value[start] == '"' || value[start] == '\''))
		return (*i = len, ft_strdup(shell, "$"));
	if (!ft_isalnum(value[len]) && value[len] != '_'
		&& value[len] != '"' && value[len] != '\'')
		return (*i = len, ft_strdup(shell, "$"));
	while (value[len] && !ft_strchr("$\"'", value[len]))
	{
		flag = 1;
		len++;
	}
	*i = len;
	if (flag)
		len++;
	p = ft_strduplen(shell, &value[start - 1], len - start);
	return (p);
}

char	*delim_dquote(t_shell *shell, char *val, int *i)
{
	int		j;
	int		start;
	char	*new_val;

	start = *i + 1;
	new_val = ft_strdup(shell, "");
	while (val[start] && val[start] != '"')
	{
		j = start;
		while (val[start] && val[start] != '"')
			start++;
		if (j != start)
			new_val = ft_strjoin(shell, new_val, ft_strduplen(shell, &val[j] \
			, start - j));
		if (val[start] == '"')
			break ;
		if (val[start] == '$')
			new_val = ft_strjoin(shell, new_val, delim_dollar(shell, val \
			, &start, TRUE));
	}
	*i = start + 1;
	return (new_val);
}

char	*delim_squote(t_shell *shell, char *value, int *i)
{
	int	len;
	int	start;

	start = *i + 1;
	len = start;
	while (value[len] && value[len] != '\'')
		len++;
	*i = len + 1;
	return (ft_strduplen(shell, &value[start], len - start));
}

char	*expand_delim(t_shell *shell, char *value)
{
	char	*new_val;
	int		i;
	int		j;

	i = 0;
	new_val = NULL;
	while (value[i])
	{
		j = i;
		while (value[i] && !ft_strchr("\"$'", value[i]))
			i++;
		if (j != i)
			new_val = ft_strjoin(shell, new_val, ft_strduplen(shell \
			, &value[j], i - j));
		if (value[i] == '\'')
			new_val = ft_strjoin(shell, new_val, delim_squote(shell \
			, value, &i));
		else if (value[i] == '"')
			new_val = ft_strjoin(shell, new_val, delim_dquote(shell \
			, value, &i));
		else if (value[i] == '$')
			new_val = ft_strjoin(shell, new_val, delim_dollar(shell, \
			value, &i, FALSE));
	}
	return (new_val);
}
