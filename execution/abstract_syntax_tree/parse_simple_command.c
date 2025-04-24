/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:03:27 by iammar            #+#    #+#             */
/*   Updated: 2025/04/24 20:14:13 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"


void free_ast(t_ast *node)
{
    if (!node)
        return;
    
    free_ast(node->left);
    free_ast(node->right);
    if(node->arguments)
        free(node->arguments);
    free(node);
}

static t_ast	*handle_bracket_content(t_dll **tokens, t_shell *shell)
{
	t_ast	*bracket_content;
	t_dll	*current;

	*tokens = (*tokens)->next;
	current = *tokens;
	while (current && !(current->bracket && current->value
			&& ft_strcmp(current->value, ")") == 0))
	{
		current->inside_parentheses = TRUE;
		current = current->next;
	}
	bracket_content = parse_logical_operators(tokens, shell);
	if (*tokens && (*tokens)->bracket)
		*tokens = (*tokens)->next;
	return (bracket_content);
}

static void	copy_token_properties(t_dll *src, t_dll *dst)
{
	dst->value = src->value;
	dst->token_type = src->token_type;
	dst->quote_type = src->quote_type;
	dst->redir_type = src->redir_type;
	dst->inside_parentheses = src->inside_parentheses;
	dst->expandable = src->expandable;
	dst->direction = src->direction;
	dst->bracket = src->bracket;
	dst->heredoc = src->heredoc;
	dst->wildcard = src->wildcard;
	dst->operator = src->operator;
	dst->prev = NULL;
	dst->next = NULL;
}

static void	add_arg_to_list(t_dll **tail, t_dll *new_arg)
{
	if (*tail)
		(*tail)->next = new_arg;
	new_arg->prev = *tail;
	*tail = new_arg;
}

static void	process_command_arguments(t_dll **tokens, t_dll **tail)
{
	t_dll	*new_arg;

	while (*tokens && (*tokens)->value && ((*tokens)->token_type == WORD
			|| (*tokens)->token_type == REDIRECTION))
	{
		new_arg = malloc(sizeof(t_dll));
		if (!new_arg)
			return ;
		copy_token_properties(*tokens, new_arg);
		add_arg_to_list(tail, new_arg);
		tail = &new_arg->next;
		*tokens = (*tokens)->next;
	}
}

t_ast	*parse_simple_command(t_dll **tokens, t_shell *shell)
{
	t_ast	*cmd_node;
	t_dll	**tail;

	if (!*tokens || !(*tokens)->value)
		return (NULL);
	if ((*tokens)->bracket)
		return (handle_bracket_content(tokens, shell));
	cmd_node = malloc(sizeof(t_ast));
	if (!cmd_node)
		return (NULL);
	cmd_node->token = *tokens;
	cmd_node->arguments = NULL;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	*tokens = (*tokens)->next;
	tail = &cmd_node->arguments;
	process_command_arguments(tokens, tail);
	return (cmd_node);
}