/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redire_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/23 09:36:20 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void    restore_fds(t_dll *token)
{
    if (!token)
        return ;
    if (token->fds[0] != STDIN_FILENO && token->fds[0] != -1)
	{
		dup2(STDIN_FILENO, 0);
		close(token->fds[0]);
		token->fds[0] = STDIN_FILENO;
	}
	if (token->fds[1] != STDOUT_FILENO && token->fds[1] != -1)
	{
		dup2(STDOUT_FILENO, 1);
		close(token->fds[1]);
		token->fds[1] = STDOUT_FILENO;
	}
}

void    identify_redirections(t_dll **tokens)
{
    t_dll *curr;

    if (!tokens || !*tokens)
        return ;
    curr = *tokens;
    while (curr)
    {
        if (ft_strnstr(curr->value, "/tmp/.heredoc_", 14))
			curr->token_type = REDIRECTION;
        if (curr->redir_type)
            curr->token_type = REDIRECTION;
        curr = curr->next;
    }
}
