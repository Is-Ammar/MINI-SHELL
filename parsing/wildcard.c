/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/10 18:27:18 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int suffix(char *name, char *val)
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

int	last_check(char *val, char *name, int i, int j)
{
	while (val[i] && val[i] == '*')
		i++;
	if (val[i] && name[j] == '\0')
		return (1);
	return (0);
}

int	check_wildcard(char *val, char *name, int i)
{
	int is_patern;
	int	pos;
	int	j;

	(1) && (j = i, is_patern = TRUE);
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
				(i = pos, j++, is_patern = FALSE);
			else
				(i++, j++);
		}
		if (is_patern == FALSE)
			return (1);
	}
	return (last_check(val, name, i, j));
}

int prefix(char *name, char *val, int *start, int *flag)
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
	while (val[i] && val[i] == '*')
		i++;
	if (val[i] == '\0')
		*flag = 1;
	return (0);
}

int	search_for_match(t_dll **tokens, t_dll *curr, char *val, char *d_name)
{
	int	i;
	int	j;
	int flag;

	i = 0;
	j = 0;
	flag = 0;
	while (val[j] && val[j] == '*')
		val++;
	if (val[j] == '\0' && *d_name == '.')
		return (1);
	if (prefix(d_name, val, &i, &flag))
		return (1);
	if (!flag && check_wildcard(val, d_name, i))
		return (1);
	if (!flag && suffix(d_name, val))
		return (1);
	add_mid_token(tokens, curr, d_name);
	return (0);
}

int	wildcard(t_dll **tokens, t_dll *curr)
{
    DIR				*dir;
	struct dirent	*dentry;
	int				match_number;

	dir = opendir(".");
	if (!dir)
		return (perror("minishell: "), 0);
	dentry = readdir(dir);
	match_number = 0;
	while (dentry != NULL)
	{
		if (!search_for_match(tokens, curr, curr->value, dentry->d_name))
			match_number++;
		dentry = readdir(dir);
	}
	if (match_number)
		remove_token(tokens, curr);
	closedir(dir);
	return (0);
}
