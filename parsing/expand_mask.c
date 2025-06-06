/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_mask.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:06:55 by habdella          #+#    #+#             */
/*   Updated: 2025/06/06 10:32:53 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*set_val(t_shell *shell, char *val, char c)
{
	char	*set;
	int		len;
	int		i;

	if (!val)
		return (ft_strdup(shell, ""));
	len = ft_strlen(val);
	set = ft_malloc(shell, (len + 1), 0);
	i = 0;
	while (i < len)
	{
		set[i] = c;
		i++;
	}
	set[i] = '\0';
	return (set);
}

char	*get_mask_str(t_shell *shell, char *value)
{
	char	*mask;
	int		i;
	int		j;

	i = 0;
	mask = ft_strdup(shell, "");
	while (value[i])
	{
		j = i;
		while (value[i] && !ft_strchr("\"$'", value[i]))
			i++;
		if (j != i)
			mask = ft_strjoin(shell, mask \
			, set_val(shell, ft_strduplen(shell, &value[j], i - j), '0'));
		if (value[i] == '\'')
			mask = ft_strjoin(shell, mask \
			, set_val(shell, single_quote(shell, value, &i), '0'));
		else if (value[i] == '"')
			mask = ft_strjoin(shell, mask \
			, set_val(shell, double_quote(shell, value, &i), '0'));
		else if (value[i] == '$')
			mask = ft_strjoin(shell, mask, set_val(shell \
			, dollar_sign(shell, value, &i, FALSE), '1'));
	}
	return (mask);
}

int	splited_tokens(t_dll *token, t_dll *last_token)
{
	t_dll	*curr;
	int		count;

	if (!token || token)
		return (0);
	count = 0;
	curr = token;
	while (curr && curr != last_token->next)
	{
		if (curr->is_splited == TRUE)
			count++;
		curr = curr->next;
	}
	return (count);
}

int	check_for_ambiguous(t_dll *head, t_dll *last_token)
{
	t_dll	*curr;
	int		ambg_num;

	if (head == last_token && head->token_type == REDIRECTION)
		return (parse_error(head->value, EAMBIGUO), 1);
	curr = head;
	ambg_num = splited_tokens(curr, last_token);
	if (curr->token_type == REDIRECTION && ambg_num == 0
		&& curr->quote_type == NONE)
	{
		return (parse_error(head->value, EAMBIGUO), 1);
	}
	while (curr && curr != last_token->next)
	{
		if (head->token_type == REDIRECTION && curr->value[0] == '\0')
		{
			parse_error(head->value, EAMBIGUO);
			return (1);
		}
		if (head->token_type == REDIRECTION && head->expandable
			&& ambg_num > 1)
			return (parse_error(head->value, EAMBIGUO), 1);
		curr = curr->next;
	}
	return (0);
}

int	expanding(t_shell *shell, t_dll **tokens, t_dll *curr, char *value)
{
	t_expand	*expand;
	t_dll		*last_token;
	int			return_val;

	expand = ft_malloc(shell, sizeof(t_expand), 0);
	expand->str = expand_env_str(shell, value);
	if (expand->str && !*expand->str)
	{
		curr->value = ft_strdup(shell, expand->str);
		return (0);
	}
	expand->mask = get_mask_str(shell, value);
	last_token = split_tok(shell, tokens, curr, expand);
	return_val = check_for_ambiguous(curr, last_token);
	curr->value = expand->str;
	remove_token(tokens, curr);
	return (return_val);
}
