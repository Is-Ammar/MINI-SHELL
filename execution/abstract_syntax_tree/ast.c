/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:22 by iammar            #+#    #+#             */
/*   Updated: 2025/06/15 16:55:09 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_ast	*abstract_segment_tree(t_shell *shell)
{
	t_ast	*abst;
	t_dll	*curr_token;

	abst = NULL;
	curr_token = shell->tokens;
	if (!curr_token || !curr_token->value)
		return (NULL);
	abst = parse_logical_operators(&curr_token, shell);
	return (abst);
}

t_ast	*parse_logical_operators(t_dll **tokens, t_shell *shell)
{
	t_ast	*result;
	t_ast	*logical_node;

	result = parse_pipe(tokens, shell);
	while (*tokens && (*tokens)->value && (*tokens)->token_type == OPERATOR)
	{
		logical_node = ft_malloc(shell, sizeof(t_ast), 0);
		logical_node->token = *tokens;
		logical_node->left = result;
		logical_node->forked = FALSE;
		*tokens = (*tokens)->next;
		logical_node->right = parse_pipe(tokens, shell);
		if (!logical_node->right)
			return (NULL);
		result = logical_node;
	}
	return (result);
}

t_ast	*parse_pipe(t_dll **tokens, t_shell *shell)
{
	t_ast	*result;
	t_ast	*pipe_node;

	result = parse_redirections(tokens, shell);
	while (*tokens && (*tokens)->value && (*tokens)->token_type == PIPE)
	{
		pipe_node = ft_malloc(shell, sizeof(t_ast), 0);
		pipe_node->token = *tokens;
		pipe_node->left = result;
		pipe_node->forked = FALSE;
		*tokens = (*tokens)->next;
		pipe_node->right = parse_redirections(tokens, shell);
		if (!pipe_node->right)
			return (NULL);
		result = pipe_node;
	}
	return (result);
}

t_ast	*parse_first_redirection(t_dll **tokens, t_shell *shell,
		t_ast *last_redir, t_ast *redir_node)
{
	t_ast	*first_redir;

	first_redir = NULL;
	while (*tokens && (*tokens)->token_type == REDIRECTION)
	{
		redir_node = ft_malloc(shell, sizeof(t_ast), 0);
		redir_node->token = *tokens;
		redir_node->left = NULL;
		redir_node->right = NULL;
		redir_node->arguments = NULL;
		redir_node->forked = FALSE;
		if (!first_redir)
			first_redir = redir_node;
		if (last_redir)
			last_redir->right = redir_node;
		last_redir = redir_node;
		*tokens = (*tokens)->next;
	}
	return (first_redir);
}

t_ast	*parse_redirections(t_dll **tokens, t_shell *shell)
{
	t_ast	*result;
	t_ast	*command_node;
	t_ast	*first_redir;
	t_ast	*last_redir;
	t_ast	*redir_node;
	t_ast	*curr_redir;

	result = NULL;
	command_node = NULL;
	first_redir = NULL;
	last_redir = NULL;
	redir_node = NULL;
	first_redir = parse_first_redirection(tokens, shell, last_redir,
			redir_node);
	command_node = parse_simple_command(tokens, shell);
	if (first_redir)
	{
		curr_redir = first_redir;
		while (curr_redir)
		{
			curr_redir->left = command_node;
			curr_redir = curr_redir->right;
		}
		result = first_redir;
	}
	else
	{
		result = command_node;
	}
	while (*tokens && (*tokens)->token_type == REDIRECTION)
	{
		redir_node = ft_malloc(shell, sizeof(t_ast), 0);
		redir_node->token = *tokens;
		redir_node->left = command_node;
		redir_node->right = NULL;
		redir_node->arguments = NULL;
		redir_node->forked = FALSE;
		if (last_redir)
			last_redir->right = redir_node;
		else
			result = redir_node;
		last_redir = redir_node;
		*tokens = (*tokens)->next;
	}
	return (result);
}
