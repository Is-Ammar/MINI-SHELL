/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/02 08:15:47 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		j;
	int		len;
	char	*p;

	if (!s1 && !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	p = malloc(len + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		p[i++] = s2[j];
		j++;
	}
	p[i] = '\0';
	return (p);
}

int	ft_stristr(char *big, char *little)
{
	int		i;
	int		j;

	i = 0;
	if (!big)
		return (-1);
	if (*little == '\0')
		return (-1);
	while (big[i])
	{
		j = 0;
		while (big[i + j] && big[i + j] == little[j])
		{
			if (little[j + 1] == '\0')
				return (i);
			j++;
		}
		i++;
	}
	return (0);
}

char	*remove_str(char *token, char *remove)
{
	char	*p;
	int		len;
	int		i;
	int		j;

	if (!token)
		return (NULL);
	if (!remove)
		return (token);
	len = ft_strlen(token) - ft_strlen(remove);
	if (len <= 0)
		return (NULL);
	p = malloc(len + 1);
	(1) && (i = 0, j = 0);
	while (token[i])
	{
		if (ft_stristr(token, remove) == i)
			i += ft_strlen(remove);
		if (ft_strlen(token) == i)
			break ;
		p[j++] = token[i];
		i++;	
	}
	p[j] = '\0';
	return (p);
}

char	*replace_str(char *token, char *name, char *env_value, int len)
{
	char	*p;
	int		l;
	int		i;
	int		j;

	l = ft_strlen(token) + ft_strlen(env_value) - len;
	p = malloc(l + 1);
	if (!p)
		return (NULL);
	(1) && (i = 0, j = 0, l = 0);
	while (token[i])
	{
		if (ft_stristr(token, name) == i)
		{
			while (env_value[l])
				p[j++] = env_value[l++];
			i += ft_strlen(name);
		}
		if (ft_strlen(token) == i)
			break ;
		p[j++] = token[i];
		i++;
	}
	p[j] = '\0';
	return (p);
}

char	*ft_strdup_expand(char *token, t_env *env, int start, int end)
{
	char	*name;
	char	*env_value;
	char	*p;
	int		len;

	if (!token)
		return (NULL);
	len = end - start + 2;
	name = ft_strduplen(&token[start], len);
	env_value = get_env_var(env, &name[1]);
	if (!env_value)
	{
		p = remove_str(token, name);
		return (p);
	}
	p = replace_str(token, name, env_value, --len);
	return (env_value);
}
