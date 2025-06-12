/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/12 13:03:46 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	single_double_checks(char *val, int *start, char c)
{
	int	i;

	i = *start;
	i++;
	while (val[i] && val[i] != c)
		i++;
	if (val[i])
		i++;
	*start = i;
}

int	check_depth_to_expand(char *val)
{
	int	i;

	i = 0;
	if (!ft_strchr(val, '*'))
		return (0);
	while (val[i])
	{
		if (val[i] == '\'')
			single_double_checks(val, &i, '\'');
		else if (val[i] == '"')
			single_double_checks(val, &i, '"');
		else if (val[i] == '*')
			return (1);
		else
			i++;
	}
	return (0);
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
		else if (!search_for_match(curr->value, dentry->d_name))
		{
			add_mid_token(shell, tokens, curr, dentry->d_name);
			match_number++;
		}
		dentry = readdir(dir);
	}
	if (match_number)
		remove_token(tokens, curr);
	closedir(dir);
	return (match_number);
}
