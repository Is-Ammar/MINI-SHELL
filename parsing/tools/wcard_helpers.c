/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcard_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/08 16:14:45 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	suffix(char *name, char *val)
{
	int	i;
	int	j;

	i = ft_strlen(val) - 1;
	j = ft_strlen(name) - 1;
	if (val[i] == '*')
		return (0);
	while (val[i] && name[j] && val[i] != '*')
	{
		if (val[i] != name[j])
			return (1);
		i--;
		j--;
	}
	return (0);
}

int	last_infix_check(char *val, char *name, int i, int j)
{
	while (val[i] && val[i] == '*')
		i++;
	if (val[i] && name[j] == '\0')
		return (1);
	return (0);
}

int	infix(char *val, char *name, int i)
{
	int	is_patern;
	int	pos;
	int	j;

	j = i;
	is_patern = TRUE;
	while (val[i] && name[j])
	{
		while (name[j] && val[i] && val[i] == '*')
			i++;
		pos = i;
		while (name[j] && val[i] && val[i] != '*')
		{
			if (val[i] == name[j] && (val[i + 1] == '\0' || val[i + 1] == '*'))
				is_patern = TRUE;
			if (val[i] != name[j])
				(1) && (i = pos, j++, is_patern = FALSE);
			else
				(1) && (i++, j++);
		}
		if (is_patern == FALSE)
			return (1);
	}
	return (last_infix_check(val, name, i, j));
}

int	prefix(char *name, char *val, int *start)
{
	int	i;

	i = 0;
	if (name[0] == '*')
		return (0);
	while (val[i] && name[i] && val[i] != '*')
	{
		if (val[i] != name[i])
			return (1);
		i++;
	}
	*start = i;
	return (0);
}

int	search_for_match(char *val, char *d_name)
{
	int	i;

	i = 0;
	if (prefix(d_name, val, &i))
		return (1);
	if (infix(val, d_name, i))
		return (1);
	if (suffix(d_name, val))
		return (1);
	return (0);
}
