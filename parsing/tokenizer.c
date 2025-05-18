/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/16 15:28:36 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*handle_expansion_var(t_shell *shell, char *input, int *i)
{
	int		len;
	char	*token;

	len = *i + 1;
	while (input[len] && !ft_strchr(METACHARS, input[len]))
		len++;
	token = ft_strduplen(shell, &input[*i], len - *i);
	*i = len;
	return (token);
}

char	*handle_quotes_bracket(t_shell *shell, char *input, int *i, int type)
{
	int		len;
	char	*token;

	len = *i + 1;
	if (type == SQUOTE)
		while (input[len] && input[len] != '\'')
			len++;
	else if (type == DQUOTE)
		while (input[len] && input[len] != '"')
			len++;
	if ((input[*i] == '\'' && input[len] == '\'')
		|| (input[*i] == '"' && input[len] == '"'))
	{
		token = ft_strduplen(shell, &input[*i], len - *i + 1);
		*i = len + 1;
	}
	else
	{
		token = ft_strduplen(shell, &input[*i], len - *i);
		*i = len;
	}
	return (token);
}

char	*get_token_val(t_shell *shell, char *input, int *index)
{
	int		i;
	char	*token;

	i = *index;
	if (input[i] == '\'')
		token = handle_quotes_bracket(shell, input, &i, SQUOTE);
	else if (input[i] == '"')
		token = handle_quotes_bracket(shell, input, &i, DQUOTE);
	else if (ft_strchr("$", input[i]))
		token = handle_expansion_var(shell, input, &i);
	else if (ft_strchr("( )\t<|>\n&", input[i]))
	{
		token = ft_strduplen(shell, &input[i], 1);
		i++;
	}
	else
	{
		while (input[i] && !ft_strchr(METACHARS, input[i]))
			i++;
		token = ft_strduplen(shell, &input[*index], i - *index);
	}
	*index = i;
	return (token);
}

t_token_type	get_token_type(char *val)
{
	if ((val[0] == '|' && val[1] == '|')
		|| (val[0] == '&' && val[1] == '&'))
		return (OPERATOR);
	if (ft_strchr("$'\"", *val))
		return (WORD);
	if (ft_strchr("<>", *val))
		return (REDIRECTION);
	if (ft_strchr("|", *val))
		return (PIPE);
	if (ft_strchr("()", *val))
		return (SYMBOL);
	if (ft_strchr("\t \n", *val))
		return (WHITESPACE);
	return (WORD);
}

t_dll	*tokenize_input(t_shell *shell, char *input)
{
	t_token_type	token_type;
	t_dll			*head;
	char			*token_val;
	int				index;

	head = NULL;
	index = 0;
	while (input[index])
	{
		token_val = get_token_val(shell, input, &index);
		if (!token_val)
			return (NULL);
		token_type = get_token_type(token_val);
		add_token(shell, &head, token_val, token_type);
	}
	return (head);
}
