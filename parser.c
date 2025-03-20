#include "smash.h"

void	check_for_merge(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr && curr->next)
	{
		Next = curr->next;
		if (Next && !ft_strncmp(curr->value, Next->value, 1))
		{
			if (curr->value[0] == '&' || curr->value[0] == '|'
				|| curr->value[0] == '<' || curr->value[0] == '>')
			{
				merge_tokens(curr, Next);
				curr->token_type = get_token_type(curr->value);
			}
		}
		curr->quote_type = get_quote_type(curr->value);
		curr = curr->next;
	}
	if (curr)
		curr->quote_type = get_quote_type(curr->value);
}

int	check_quotes(t_dll *tokens)
{
	t_dll	*curr;
	int		i;

	curr = tokens;
	while (curr)
	{
		i = 0;
		if (curr->quote_type == SQUOTE)
		{
			while (curr->value[i])
				i++;
			if (curr->value[--i] != '\'' || ft_strlen(curr->value) == 1)
				return (Error(curr->value, EQUOTES), 1);
		}
		else if (curr->quote_type == DQUOTE)
		{
			while (curr->value[i])
				i++;
			if (curr->value[--i] != '"' || ft_strlen(curr->value) == 1)
				return (Error(curr->value, EQUOTES), 1);
		}
		curr = curr->next;
	}
	return (0);
}

void	merge_quotes(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr && curr->next)
	{
		Next = curr->next;
		if ((curr->token_type == WORD && Next->token_type == WORD)
			|| (curr->quote_type != NONE && Next->quote_type != NONE))
		{
			merge_tokens(curr, Next);
			curr->token_type = get_token_type(curr->value);
			continue ;
		}
		curr = curr->next;
	}
}

int	check_brackets(t_dll *tokens)
{
	t_dll	*curr;
	int		count;

	curr = tokens;
	count = 0;
	while (curr)
	{
		if (curr->value[0] == '(')
			count++;
		else if (curr->value[0] == ')')
			count--;
		curr = curr->next;
	}
	if (count)
		return (Error(tokens->value, EBRACKET), 1);
	return (0);
}

void	remove_spaces(t_dll *tokens)
{
	t_dll	*curr;

	if (!tokens)
		return ;
	curr = tokens;
	while (curr)
	{
		if (curr->token_type == WHITESPACE)
			remove_token(&tokens, curr);
		curr = curr->next;
	}	
}

int	parse_input(t_dll *tokens)
{
	check_for_merge(tokens);
	if (check_quotes(tokens))
		return (1);
	merge_quotes(tokens);
	if (check_brackets(tokens))
		return (1);
	remove_spaces(tokens);
	return (0);
}