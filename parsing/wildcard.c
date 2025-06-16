/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/16 11:14:13 by habdella         ###   ########.fr       */
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

t_dll	*add_matched(t_shell *shell, t_dll **head, t_dll *token, char *val)
{
	t_dll	*nxt;
	t_dll	*new_token;

	new_token = create_token_list(shell);
	new_token->value = ft_strdup(shell, val);
	new_token->token_type = WORD;
	if (!*head)
	{
		*head = new_token;
		return (new_token);
	}
	if (!token)
		return (*head);
	nxt = token->next;
	token->next = new_token;
	new_token->prev = token;
	new_token->next = nxt;
	if (nxt)
		nxt->prev = new_token;
	return (new_token);
}

int	wildcard(t_shell *shell, t_dll **tokens, t_dll *curr, char *old_val)
{
	DIR				*dir;
	struct dirent	*dentry;
	t_dll			*token;
	int				match_number;

	token = curr;
	match_number = 0;
	dir = opendir(".");
	dentry = readdir(dir);
	while (dentry != NULL)
	{
		if (dentry->d_name[0] == '.' && curr->value[0] != '.')
			;
		else if (search_match(shell, curr->value, dentry->d_name, old_val))
		{
			token = add_matched(shell, tokens, token, dentry->d_name);
			match_number++;
		}
		dentry = readdir(dir);
	}
	if (match_number)
		remove_token(tokens, curr);
	closedir(dir);
	return (match_number);
}
