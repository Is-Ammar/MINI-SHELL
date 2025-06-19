/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 02:33:56 by iammar            #+#    #+#             */
/*   Updated: 2025/06/19 23:47:20 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_ast	*create_redir_node(t_dll **tokens, t_shell *shell)
{
	t_ast	*redir_node;

	redir_node = ft_malloc(shell, sizeof(t_ast), 0);
	redir_node->token = *tokens;
	redir_node->left = NULL;
	redir_node->right = NULL;
	redir_node->arguments = NULL;
	*tokens = (*tokens)->next;
	return (redir_node);
}

void	link_redirections_to_command(t_ast *first_redir, t_ast *command_node)
{
	t_ast	*curr_redir;

	curr_redir = first_redir;
	while (curr_redir)
	{
		curr_redir->left = command_node;
		curr_redir = curr_redir->right;
	}
}

void	handle_prefix_redirections(t_dll **tokens, t_shell *shell,
		t_ast **first_redir, t_ast **last_redir)
{
	t_ast	*redir_node;

	while (*tokens && (*tokens)->token_type == REDIRECTION)
	{
		redir_node = create_redir_node(tokens, shell);
		if (!*first_redir)
			*first_redir = redir_node;
		if (*last_redir)
			(*last_redir)->right = redir_node;
		*last_redir = redir_node;
	}
}

t_ast	*handle_suffix_redirections(t_dll **tokens, t_shell *shell,
		t_redir_context *ctx, t_ast *result)
{
	t_ast	*redir_node;

	while (*tokens && (*tokens)->token_type == REDIRECTION)
	{
		redir_node = create_redir_node(tokens, shell);
		redir_node->left = ctx->command_node;
		if (*(ctx->last_redir))
			(*(ctx->last_redir))->right = redir_node;
		else
			result = redir_node;
		*(ctx->last_redir) = redir_node;
	}
	return (result);
}

t_ast	*parse_redirections(t_dll **tokens, t_shell *shell)
{
	t_ast			*result;
	t_ast			*command_node;
	t_ast			*first_redir;
	t_ast			*last_redir;
	t_redir_context	ctx;

	result = NULL;
	first_redir = NULL;
	last_redir = NULL;
	handle_prefix_redirections(tokens, shell, &first_redir, &last_redir);
	command_node = parse_simple_command(tokens, shell);
	if (first_redir)
	{
		link_redirections_to_command(first_redir, command_node);
		result = first_redir;
	}
	else
		result = command_node;
	ctx = (t_redir_context){command_node, &last_redir};
	result = handle_suffix_redirections(tokens, shell, &ctx, result);
	return (result);
}
