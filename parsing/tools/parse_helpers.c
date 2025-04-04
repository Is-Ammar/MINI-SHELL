/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/04 08:28:07 by habdella         ###   ########.fr       */
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

static void	helper(long nb, char *p)
{
	int		i;
	int		j;
	char	arr[11];

	i = 0;
	j = 0;
	if (nb == 0)
		p[j++] = '0';
	if (nb < 0)
	{
		p[j++] = '-';
		nb *= -1;
	}
	while (nb > 0)
	{
		arr[i++] = (nb % 10) + 48;
		nb /= 10;
	}
	while (i >= 0)
		p[j++] = arr[--i];
	p[--j] = '\0';
}

char	*ft_itoa(int n)
{
	long	nb;
	char	*itoa;
	int		size;

	nb = n;
	size = 0;
	if (n < 0)
		n = -n;
	while (n)
	{
		size++;
		n /= 10;
	}
	itoa = malloc(size + 2);
	if (!itoa)
		return (NULL);
	helper(nb, itoa);
	return (itoa);
}

char	*ft_strdup_expand(char *value, t_env *env)
{
	char	*name;
	char	*env_value;

	if (!value)
		return (NULL);
	if (value[0] == '"')
	{
		name = ft_strduplen(&value[1], ft_strlen_quotes(value));
		env_value = get_env_var(env, &name[1]);
	}
	else
	{
		name = ft_strdup(value);
		env_value = get_env_var(env, name);
	}
	free(name);
	if (!env_value)
		return ("");
	return (ft_strdup(env_value));
}