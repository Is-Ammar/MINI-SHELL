/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/16 08:06:46 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int    handle_redirect(char *value, t_dll *_Next)
{
    if (!ft_strcmp(value, ">>"))
	{
		_Next->redir_type = APPEND;
		return (1);
	}
	if (!ft_strcmp(value, "<"))
	{
		_Next->redir_type = READ;
		return (1);
	}	
	if (!ft_strcmp(value, ">"))
	{
		_Next->redir_type = WRITE;
		return (1);
	}
	return (0);
}

void	redirections(t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*_Next;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		_Next = curr->next;
		if (curr->direction == LEFT || curr->direction == RIGHT)
		{
			if (handle_redirect(curr->value, _Next))
				remove_token(tokens, curr);
		}
		curr = _Next;
	}
}
