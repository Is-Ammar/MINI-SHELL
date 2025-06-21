/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/20 13:53:22 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	clear_non_env(t_shell *shell)
{
	t_gc	*curr;
	t_gc	*nxt;

	if (!shell->g_collect)
		return ;
	curr = shell->g_collect;
	while (curr)
	{
		nxt = curr->next;
		if (curr->is_env == FALSE)
		{
			free_node(&shell->g_collect, curr);
			curr = NULL;
		}
		curr = nxt;
	}
	free(curr);
}

void	burn_garbage(t_shell *shell)
{
	t_gc	*curr;
	t_gc	*nxt;

	if (!shell->g_collect)
		return ;
	curr = shell->g_collect;
	while (curr)
	{
		nxt = curr->next;
		free(curr->address);
		free(curr);
		curr = nxt;
	}
	free(curr);
	shell->g_collect = NULL;
}

void	add_to_garbage(t_shell *shell, void *ptr, int is_env)
{
	t_gc	*node;
	t_gc	*curr;

	node = malloc(sizeof(t_gc));
	if (!node)
		return (burn_garbage(shell));
	node->is_env = is_env;
	node->address = ptr;
	node->next = NULL;
	if (!shell->g_collect)
	{
		shell->g_collect = node;
		return ;
	}
	curr = shell->g_collect;
	while (curr->next)
		curr = curr->next;
	curr->next = node;
}

void	*ft_malloc(t_shell *shell, size_t size, int is_env)
{
	void	*p;

	p = malloc(size);
	if (!p)
	{
		burn_garbage(shell);
		exit(1);
	}
	add_to_garbage(shell, p, is_env);
	return (p);
}

void	clean_exit(t_shell *shell, int exit_code)
{
	burn_garbage(shell);
	exit(exit_code);
}
