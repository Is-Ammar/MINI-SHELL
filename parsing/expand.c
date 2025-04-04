/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/04 08:38:56 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "parsing.h"

char	*dollar_sign(char *value, int *i, t_env *env, int e_code)
{
	int 	len;
	int 	start;
	char	*p;

	start = *i + 1;
	len = start;
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
		len++;
	if (value[len] == '?')
		return (*i = len + 1, ft_itoa(e_code));
	*i = len;
	p = ft_strduplen(&value[start], len - start);
	return (ft_strdup_expand(p, env));
}

char	*double_quote(char *value, int *i, t_env *env, int e_code)
{
	int len;
	int flag;
	int start;

	(void)e_code;
	start = *i + 1;
	len = start;
	flag = 0;
	while (value[len] && !ft_strchr("\"$", value[len]))
		len++;
	*i = len + 1;
	if (value[len] == '"')
		return (ft_strduplen(&value[start], len - start));
	while (value[len] && !ft_strchr("\"$", value[len]))
		len++;
	if (value[len] == '"')
		*i = len + 1;
	else
		*i = len;
	return (ft_strdup_expand(&value[start], env));
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

char	*expanding(char *value, t_env *env, int e_code)
{
	char	*new_val;
	char	*temp;
	int		i;
	int		j;

	(1) && (new_val = ft_strdup(""), i = 0);
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
