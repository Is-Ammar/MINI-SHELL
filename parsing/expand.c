/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/10 16:00:39 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*ft_strdup_expand(t_shell *shell, char *value)
{
	t_env	*env;
	char	*name;
	char	*env_value;

	if (!value)
		return (NULL);
	env = shell->env_list;
	name = ft_strdup(shell, value);
	env_value = get_env_var(shell, env, name);
	free(name);
	if (!env_value)
		return ("");
	return (ft_strdup(shell, env_value));
}

char	*dollar_sign(t_shell *shell, char *value, int *i)
{
	int		len;
	int		start;
	int		e_code;
	char	*p;

	p = NULL;
	e_code = shell->exit_code;
	start = *i + 1;
	len = start;
	if (value[len] == '$' || value[len] == '\0')
		return (*i = len, ft_strdup(shell, "$"));
	if (value[len] == '?')
		return (*i = len + 1, ft_itoa(shell, e_code));
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
		len++;
	*i = len;
	p = ft_strduplen(shell, &value[start], len - start);
	return (ft_strdup_expand(shell, p));
}

char	*double_quote(t_shell *shell, char *val, int *i)
{
	int		j;
	int		start;
	char	*new_val;

	start = *i + 1;
	new_val = ft_strdup(shell, "");
	while (val[start] && val[start] != '"')
	{
		j = start;
		while (val[start] && !ft_strchr("\"$", val[start]))
			start++;
		if (j != start)
			new_val = ft_strjoin(shell, new_val, ft_strduplen(shell, &val[j] \
			, start - j));
		if (val[start] == '"')
			break ;
		if (val[start] == '$')
			new_val = ft_strjoin(shell, new_val, dollar_sign(shell, val \
			, &start));
	}
	*i = start + 1;
	return (new_val);
}

char	*single_quote(t_shell *shell, char *value, int *i)
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

char	*expand_env_vars(t_shell *shell, char *value)
{
	char	*new_val;
	int		i;
	int		j;

	i = 0;
	new_val = ft_strdup(shell, "");
	while (value[i])
	{
		j = i;
		while (value[i] && !ft_strchr("\"$'", value[i]))
			i++;
		if (j != i)
			new_val = ft_strjoin(shell, new_val, ft_strduplen(shell, &value[j] \
			, i - j));
		if (value[i] == '\'')
			new_val = ft_strjoin(shell, new_val, single_quote(shell, value \
			, &i));
		else if (value[i] == '"')
			new_val = ft_strjoin(shell, new_val, double_quote(shell, value \
			, &i));
		else if (value[i] == '$')
			new_val = ft_strjoin(shell, new_val, dollar_sign(shell, value, &i));
	}
	return (new_val);
}
