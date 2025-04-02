/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/02 10:54:48 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "parsing.h"

char	*get_val(char *value, int *index, t_env *env)
{
	char	*temp_val;
	int		i;
	int		start;

	i = *index;
	start = i;
	i = i + 1;
	while (value[i] && !ft_strchr("'\"$", value[i]))
		i++;
	temp_val = ft_strdup_expand(value, env, start, i - 1);
	*index = *index + i;
	return (temp_val);
}

char	*expanding(char *value, t_env *env)
{
	char	*new_val;
	char	*temp_val;
	// char	*temp;
	int		(i), (j);
	int		flag;

	(1) && (new_val = NULL, i = 0);
	while (value[i])
	{
		flag = 0;
		j = i;
		while (value[i] && !ft_strchr("\"$'", value[i]))
		{
			if (value[i] == '\'')
				flag = 1;
			i++;
		}
		if (!flag)
			temp_val = get_val(value, &i, env);
		else
			temp_val = ft_strduplen(&value[j], i);
		// ft_printf("%s\n", temp_val);
		new_val = ft_strjoin(new_val, temp_val);
		i++;
	}
	return (new_val);
}

// char	*handle_expand_quote(char *token, int i)
// {
// 	int	j;

//     if ()
// 	if (ft_strchr("\"'", token[i]) && token[i + 1] && token[i + 1] != '$')
// 	{
// 		if (token[i] == )
// 	}
// 	while (token[i])
// 	{
// 		if (ft_strchr("\"'", token[i]) && token[i + 1] == '$')
// 		{
// 			if (token[i] == )
// 		}
// 	}
// }

char	*expand(char *token, t_env *env)
{
	char	*p;
	// char	*tmp;
	int 	i;
	int 	j;

	(void)env;
	p = malloc(1024);
	if (!p)
		return (p);
	(1) && (i = 0, j = 0);
	while (token[i])
	{
		while (token[i] && token[i] != '$')
		{
			p[j++] = token[i];
			i++;
		}
		if (token[i - 1] && (token[i - 1] == '"' || token[i - 1] == '\''))
		{
			// tmp = token;
			// token = handle_expand_quote(token, i);
			// free(tmp);
		}
		if (ft_strchr(token, '$') && token[i] == '$')
		{
			// tmp = token;
			// token = handle_without_quote(token, i);
			// free(tmp);
		}
		i++;
	}
    p = ft_strdup(token);
	return (p);
}
