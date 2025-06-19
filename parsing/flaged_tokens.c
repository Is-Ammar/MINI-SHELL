/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flaged_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:23:37 by habdella          #+#    #+#             */
/*   Updated: 2025/06/19 14:48:10 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	forked_pipe(t_dll *tokens)
{
	t_dll	*curr;
	t_dll	*tmp;

	curr = tokens;
	while (curr)
	{
		if (curr->token_type == PIPE)
		{
			if (curr->next && curr->next->token_type == WORD)
				curr->next->fork = TRUE;
			tmp = curr->prev;
			while (tmp)
			{
				if ((tmp->token_type == WORD && !tmp->prev)
					|| (tmp->token_type == WORD && tmp->prev != WORD))
					tmp->fork = TRUE;
				tmp = tmp->prev;
			}
		}
		curr = curr->next;
	}
}

void	forked_subshell(t_dll *tokens)
{
	t_dll	*curr;
	int		fork;

	curr = tokens;
	fork = 0;
	if (find_token(tokens, SYMBOL) == NULL)
		return ;
	while (curr)
	{
		if (curr->value[0] == '(')
			fork++;
		else if (curr->value[0] == ')')
			fork--;
		else
			curr->fork = fork;
		curr = curr->next;
	}
}

void	flaged_tokens(t_dll *tokens)
{
	forked_subshell(tokens);
	forked_pipe(tokens);
}
