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

void	remove_token(t_dll **head, t_dll *token)
{
	if (!head || !*head || !token)
		return ;
	if (*head == token)
		*head = token->next;
	if (token->prev)
		token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	free(token->value);
	free(token);
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
