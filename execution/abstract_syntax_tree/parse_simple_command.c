/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:03:27 by iammar            #+#    #+#             */
/*   Updated: 2025/06/19 23:31:58 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	copy_token_properties(t_shell *shell, t_dll *src, t_dll *dst)
{
	dst->value = ft_strdup(shell, src->value);
	dst->token_type = src->token_type;
	dst->quote_type = src->quote_type;
	dst->redir_type = src->redir_type;
	dst->expandable = src->expandable;
	dst->expandoc = src->expandoc;
	dst->is_splited = src->is_splited;
	dst->direction = src->direction;
	dst->bracket = src->bracket;
	dst->heredoc = src->heredoc;
	dst->wildcard = src->wildcard;
	dst->operator = src->operator;
	dst->fork = src->fork;
	dst->prev = NULL;
	dst->next = NULL;
}

void	add_arg_to_list(t_dll **tail, t_dll *new_arg)
{
	if (*tail)
		(*tail)->next = new_arg;
	new_arg->prev = *tail;
	*tail = new_arg;
}

static void	process_command_arguments(t_shell *shell, t_dll **tokens,
		t_ast *cmd_node, t_dll **tail)
{
	t_dll	*curr;
	t_dll	*new_arg;

	curr = *tokens;
	while (curr && (curr->token_type == WORD
			|| curr->token_type == REDIRECTION))
	{
		if (curr->token_type == REDIRECTION)
		{
			if (curr)
				curr = curr->next;
			continue ;
		}
		if (cmd_node->token != curr)
		{
			new_arg = ft_malloc(shell, sizeof(t_dll), 0);
			copy_token_properties(shell, curr, new_arg);
			add_arg_to_list(tail, new_arg);
			remove_token(tokens, curr);
			tail = &new_arg->next;
		}
		curr = curr->next;
	}
	if (*tokens)
		*tokens = (*tokens)->next;
}

t_ast	*fill_command(t_shell *shell, t_dll **tokens)
{
	t_ast	*cmd_node;

	cmd_node = ft_malloc(shell, sizeof(t_ast), 0);
	cmd_node->token = find_command(shell, *tokens);
	cmd_node->arguments = NULL;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	return (cmd_node);
}

t_ast	*parse_simple_command(t_dll **tokens, t_shell *shell)
{
	t_ast	*cmd_node;
	t_dll	**tail;
	t_dll	*first_token;
	t_dll	*start_tokens;

	first_token = *tokens;
	if (!*tokens || !(*tokens)->value)
		return (NULL);
	if ((*tokens)->bracket)
		return (handle_bracket_content(tokens, shell));
	if (first_token->token_type != WORD)
		return (NULL);
	cmd_node = fill_command(shell, tokens);
	tail = &cmd_node->arguments;
	start_tokens = *tokens;
	process_command_arguments(shell, tokens, cmd_node, tail);
	if (cmd_node->token)
		cmd_node->token->next = NULL;
	if (!cmd_node->token && start_tokens
		&& (start_tokens->token_type == REDIRECTION))
		cmd_node->token = first_token;
	return (cmd_node);
}
