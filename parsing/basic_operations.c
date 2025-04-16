/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/13 15:08:58 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_dll	*create_token_list(void)
{
	t_dll	*head;

	head = malloc(sizeof(t_dll));
	if (!head)
		return (NULL);
	head->value = NULL;
	head->token_type = WHITESPACE;
	head->quote_type = NONE;
	head->expandable = 0;
	head->direction = 0;
	head->bracket = 0;
	head->heredoc = 0;
	head->wildcard = 0;
	head->operator = 0;
	head->prev = NULL;
	head->next = NULL;
	return (head);
}

void	add_token(t_dll **head, char *val, t_token_type token_type)
{
	t_dll *token;
	t_dll *curr;

	token = create_token_list();
	token->value = val;
	token->token_type = token_type;
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

void	add_mid_token(t_dll **head, t_dll *token, char *val)
{
	t_dll *_Next;
	t_dll *new_token;

	new_token = create_token_list();
	new_token->value = ft_strdup(val);
	new_token->token_type = WORD;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	if (!token)
		return ;
	_Next = token->next;
	token->next = new_token;
	new_token->prev = token;
	new_token->next = _Next;
	if (_Next)
		_Next->prev = new_token;
}

void	remove_token(t_dll **head, t_dll *remove)
{
	t_dll	*prv;
	t_dll	*nxt;

	if (!head || !*head || !remove)
		return ;
	if (*head == remove && remove->next == NULL)
	{
		free(remove->value);
		free(remove);
		*head = NULL;
		return ;
	}
	prv = remove->prev;
	nxt = remove->next;
	if (prv)
		prv->next = nxt;
	if (nxt)
		nxt->prev = prv;
	if (*head == remove)
		*head = nxt;
	free(remove->value);
	free(remove);
}

void	free_token_list(t_dll **head)
{
	t_dll	*curr;
	t_dll	*Next;

	if (!head || !*head)
		return ;
	curr = *head;
	while (curr)
	{
		Next = curr->next;
		remove_token(head, curr);
		curr = Next;
	}
	*head = NULL;
}
