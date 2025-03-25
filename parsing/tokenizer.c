/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/25 01:02:21 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*handle_expansion_var(char *input)
{
	int		len;
	char	*token;

	len = 1;
	while (input[len] && !ft_strchr(METACHARS, input[len]))
		len++;
	token = ft_strduplen(input, len);
	return (token);
}

char    *handle_quotes_bracket(char *input, int type)
{
	int		len;
	char	*token;

	len = 1;
	if (type == SQUOTE)
		while (input[len] && input[len] != '\'')
			len++;
	else if (type == DQUOTE)
		while (input[len] && input[len] != '"')
			len++;
	len++;
	token = ft_strduplen(input, len);
	return (token);
}

char	*get_val(char *input)
{
	int		len;
	char	*token;

	len = 0;
	if (input[len] == '\'')
	    token = handle_quotes_bracket(input, SQUOTE);
	else if (input[len] == '"')
		token = handle_quotes_bracket(input, DQUOTE);
	else if (ft_strchr("$", input[len]))
		token = handle_expansion_var(input);
	else if (ft_strchr("( )\t<|>\n&", input[len]))
		token = ft_strduplen(input, 1);
	else
	{
		while (input[len] && !ft_strchr(METACHARS, input[len]))
			len++;
		token = ft_strduplen(input, len);
	}
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

t_dll	*tokenize_input(char *input)
{
	char			*token_val;
	t_dll			*head;
	t_token_type	token_type;

	head = create_token_list();
	while (*input)
	{
		token_val = get_val(input);
		if (!token_val)
			return (NULL);
		token_type = get_token_type(token_val);
		add_token(&head, token_val, token_type);
		input += ft_strlen(token_val);
	}
	remove_token(&head, head);
	return (head);
}
