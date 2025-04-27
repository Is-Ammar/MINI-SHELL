/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/27 13:27:38 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	burn_garbage(t_shell *shell)
{
	t_gc	*curr;
	t_gc	*nxt;

	curr = shell->g_collect;
	if (!shell || !curr)
		return ;
	while (curr)
	{
		nxt = curr->next;
		if (curr->address)
			free(curr->address);
		if (curr)
			free(curr);
		curr = nxt;
	}
	shell->g_collect = NULL;
}

void	add_to_garbage(t_shell *shell, void *ptr)
{
	t_gc	*head;

	head = malloc(sizeof(t_gc));
	if (!head)
		return (burn_garbage(shell));
	head->address = ptr;
	head->next = shell->g_collect;
	shell->g_collect = head;
}

void	*ft_malloc(t_shell *shell, size_t size)
{
	void	*p;

	p = malloc(size);
	if (!p)
	{
		burn_garbage(shell);
		exit(1);
	}
	add_to_garbage(shell, p);
	return (p);
}
