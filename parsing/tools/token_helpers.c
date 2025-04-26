/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/25 11:07:21 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	ft_strlen_quotes(const char *s)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != '"' && s[i] != '\'')
			len++;
		i++;
	}
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (c == 0)
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strduplen(char *input, int len)
{
	int		i;
	char	*p;

	if (!input || len <= 0)
		return (NULL);
	p = malloc(len + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		p[i] = input[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

char	*ft_strdup_quotes(char *token)
{
	int		i;
	int		j;
	int		len;
	char	*p;

	if (!token)
		return (NULL);
	i = 0;
	j = 0;
	len = ft_strlen(token) - 1;
	p = malloc(len);
	if (!p)
		return (NULL);
	while (token[i])
	{
		if (i != 0 && i != len)
			p[j++] = token[i];
		i++;
	}
	p[j] = '\0';
	return (p);
}
