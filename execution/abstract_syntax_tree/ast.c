/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:22 by iammar            #+#    #+#             */
/*   Updated: 2025/06/17 02:34:48 by iammar           ###   ########.fr       */
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
