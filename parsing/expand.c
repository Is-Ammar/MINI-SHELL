/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/24 14:29:06 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "parsing.h"

char	*ft_strdup_expand(char *value, t_env *env)
{
	char	*name;
	char	*env_value;

	if (!value)
		return (NULL);
	name = ft_strdup(value);
	env_value = get_env_var(env, name);
	free(name);
	if (!env_value)
		return ("");
	return (ft_strdup(env_value));
}

char	*dollar_sign(char *value, int *i, t_env *env, int e_code)
{
	int 	len;
	int 	start;
	char	*p;

	start = *i + 1;
	len = start;
	if (value[len] == '?')
		return (*i = len + 1, ft_itoa(e_code));
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
		len++;
	*i = len;
	p = ft_strduplen(&value[start], len - start);
	return (ft_strdup_expand(p, env));
}

char	*double_quote(char *val, int *i, t_env *env, int e_code)
{
	int		j;
	int		start;
	char	*temp_val;
	char	*new_val;

	start = *i + 1;
	new_val = ft_strdup("");
	while (val[start] && val[start] != '"')
	{
		j = start;
		while (val[start] && !ft_strchr("\"$", val[start]))
			start++;
		temp_val = new_val;
		if (j != start)
			new_val = ft_strjoin(new_val, ft_strduplen(&val[j], start - j));
		if (val[start] == '"')
			break ;
		if (val[start] == '$')
			new_val = ft_strjoin(new_val, dollar_sign(val, &start, env, e_code));
		if (temp_val)
			free(temp_val);
	}
	*i = start + 1;
	return (new_val);
}

char	*single_quote(char *value, int *i)
{
	int	len;
	int	start;

	start = *i + 1;
	len = start;
	while (value[len] && value[len] != '\'')
		len++;
	*i = len + 1;
	return (ft_strduplen(&value[start], len - start));	
}

char	*expand_env_vars(char *value, t_env *env, int e_code)
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
		while (value[i] && !ft_strchr("\"$'", value[i]))
			i++;
		temp = new_val;
		if (j != i)
			new_val = ft_strjoin(new_val, ft_strduplen(&value[j], i - j));
		if (value[i] == '\'')
			new_val = ft_strjoin(new_val, single_quote(value, &i));
		else if (value[i] == '"')
			new_val = ft_strjoin(new_val, double_quote(value, &i, env, e_code));
		else if (value[i] == '$')
			new_val = ft_strjoin(new_val, dollar_sign(value, &i, env, e_code));
		if (temp)
			free(temp);
	}
	return (new_val);
}
