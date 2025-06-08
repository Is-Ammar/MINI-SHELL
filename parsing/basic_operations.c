/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/08 11:43:04 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_dll	*create_token_list(t_shell *shell)
{
	t_dll	*head;

	head = ft_malloc(shell, sizeof(t_dll), 0);
	head->value = NULL;
	head->token_type = WHITESPACE;
	head->quote_type = NONE;
	head->inside_parentheses = 0;
	head->redir_type = 0;
	head->expandable = 0;
	head->is_splited = 0;
	head->direction = 0;
	head->bracket = 0;
	head->heredoc = 0;
	head->expandoc = 0;
	head->wildcard = 0;
	head->operator = 0;
	head->prev = NULL;
	head->next = NULL;
	return (head);
}

void	add_token(t_shell *shell, t_dll **head, char *val, t_token_type t_type)
{
	t_dll	*token;
	t_dll	*curr;

	token = create_token_list(shell);
	token->value = val;
	token->token_type = t_type;
	if (!*head)
	{
		*head = token;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = token;
	token->prev = curr;
	token->next = NULL;
}

void	add_mid_token(t_shell *shell, t_dll **head, t_dll *token, char *val)
{
	t_dll	*nxt;
	t_dll	*new_token;

	new_token = create_token_list(shell);
	new_token->value = ft_strdup(shell, val);
	new_token->token_type = token->token_type;
	new_token->expandable = token->expandable;
	new_token->expandoc = token->expandoc;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	if (!token)
		return ;
	nxt = token->next;
	token->next = new_token;
	new_token->prev = token;
	new_token->next = nxt;
	if (nxt)
		nxt->prev = new_token;
}

void	remove_token(t_dll **head, t_dll *remove)
{
	t_dll	*curr;

	if (!head || !*head)
		return ;
	curr = *head;
	if (curr == remove)
	{
		*head = curr->next;
		remove_token(head, remove);
	}
	else
	{
		curr = *head;
		remove_token(&curr->next, remove);
	}
}

void	free_node(t_gc **head, t_gc *remove)
{
	t_gc	*curr;

	if (!head || !*head)
		return ;
	curr = *head;
	if (curr == remove)
	{
		*head = curr->next;
		free(curr->address);
		free(curr);
		free_node(head, remove);
	}
	else
	{
		curr = *head;
		free_node(&curr->next, remove);
	}
}
