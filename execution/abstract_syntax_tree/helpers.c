/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:17:50 by iammar            #+#    #+#             */
/*   Updated: 2025/06/21 14:43:32 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	set_last_cmd_env(t_shell *shell)
{
	t_dll	*current;

	current = shell->ast->arguments;
	if (current)
	{
		while (current)
		{
			if (!current->next)
				break ;
			current = current->next;
		}
		set_env_var(shell, &shell->env_list, "_", current->value);
		return ;
	}
	set_env_var(shell, &shell->env_list, "_", shell->ast->token->value);
}

void	additional_wildcard(t_shell *shell)
{
	t_dll	*curr;
	t_dll	*nxt;

	curr = shell->ast->arguments;
	while (curr)
	{
		nxt = curr->next;
		if (curr->wildcard)
			wildcard(shell, &shell->ast->arguments, curr, curr->value);
		curr = nxt;
	}
}
