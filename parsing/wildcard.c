/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/09 08:55:14 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	double_quote_check(char *val, int i)
{
	i += 1;
	while (val[i] && val[i] != '"')
	{
		if (val[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

int	single_quote_check(char *val, int i)
{
	i += 1;
	while (val[i] && val[i] != '\'')
	{
		if (val[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

int	check_depth_to_expand(char *val)
{
	int	i;

	if (!ft_strchr(val, '*'))
		return (1);
	if (!ft_strchr(val, '\'') && !ft_strchr(val, '"'))
		return (0);
	i = 0;
	while (val[i])
	{
		if (val[i] == '\'' || val[i] == '"')
		{
			if (val[i] == '\'' && !single_quote_check(val, i))
				return (0);
			if (val[i] == '"' && !double_quote_check(val, i))
				return (0);
			i += 1;
			while (val[i] && (val[i] != '\'' || val[i] != '"'))
				i++;
		}
		i++;
	}
	return (1);
}

int	hidden_files(char *val, char *name)
{
	int	i;
	int	j;
	int	point_count;

	(1) && (i = 0, j = 0, point_count = 0);
	if (!ft_strcmp(".", name) || !ft_strcmp("..", name))
		return (1);
	if (name[0] == '.' && !ft_strchr(val, '.'))
		return (1);
	while (val[i] && val[i] == '*')
		i++;
	if (val[i] == '\0' && name[0] == '.')
		return (1);
	if (i != 0 && val[i] && val[i] == '.')
	{
		while (name[j])
		{
			if (name[j] && name[j] == '.')
				point_count++;
			j++;
		}
		if (name[0] == '.' && point_count == 1)
			return (1);
	}
	return (0);
}

int	wildcard(t_shell *shell, t_dll **tokens, t_dll *curr)
{
	DIR				*dir;
	struct dirent	*dentry;
	int				match_number;

	match_number = 0;
	dir = opendir(".");
	if (!dir)
		return (perror("minishell: "), 1);
	dentry = readdir(dir);
	while (dentry != NULL)
	{
		if (hidden_files(curr->value, ft_strdup(shell, dentry->d_name)))
			;
		else if (!search_for_match(shell, curr, curr->value, dentry->d_name))
			match_number++;
		dentry = readdir(dir);
	}
	if (match_number)
		remove_token(tokens, curr);
	closedir(dir);
	return (match_number);
}
