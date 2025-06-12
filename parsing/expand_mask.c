/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_mask.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:06:55 by habdella          #+#    #+#             */
/*   Updated: 2025/06/12 11:50:37 by habdella         ###   ########.fr       */
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

	if (!token)
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

int	expanding(t_shell *shell, t_dll **tokens, t_dll *curr, char *value)
{
	t_expand	*expand;
	t_dll		*last_token;

	expand = ft_malloc(shell, sizeof(t_expand), 0);
	expand->str = expand_env_str(shell, value);
	expand->mask = get_mask_str(shell, value);
	last_token = split_tok(shell, tokens, curr, expand);
	remove_additonal_chars(shell, curr, last_token);
	if (curr->token_type == REDIRECTION
		&& splited_tokens(curr, last_token) != 1)
		return (parse_error(curr->value, EAMBIGUO), 1);
	curr->value = remove_char127(shell, expand->str);
	remove_token(tokens, curr);
	return (0);
}
