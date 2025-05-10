/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/10 16:20:33 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	identify_redirections(t_dll **tokens)
{
	t_dll	*curr;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr)
	{
		if (ft_strnstr(curr->value, "/tmp/.heredoc_", 14))
		{
			curr->token_type = REDIRECTION;
			curr->redir_type = READ;
		}
		curr = curr->next;
	}
}
