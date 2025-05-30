/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_mask.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:06:55 by habdella          #+#    #+#             */
/*   Updated: 2025/05/29 09:08:28 by habdella         ###   ########.fr       */
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
	set = ft_malloc(shell, (len + 1));
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
			mask = ft_strjoin(shell, mask \
			, set_val(shell, dollar_sign(shell, value, &i, FALSE), '1'));
	}
	return (mask);
}

char	*expanding(t_shell *shell, t_dll **tokens, t_dll *curr, char *value)
{
	char	*expanded;
	char	*mask;

	expanded = expand_env_str(shell, value);
	mask = get_mask_str(shell, value);
	split_token(shell, tokens, curr, expanded, mask);
	return (expanded);
}
