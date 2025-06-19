/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 22:51:23 by iammar            #+#    #+#             */
/*   Updated: 2025/06/19 23:58:47 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_ast	*create_subshell_node(t_ast *content, t_shell *shell)
{
	t_ast	*subshell_node;
	t_dll	*subshell_token;

	subshell_node = ft_malloc(shell, sizeof(t_ast), 0);
	subshell_token = ft_malloc(shell, sizeof(t_dll), 0);
	subshell_token->value = ft_strdup(shell, "(subshell)");
	subshell_token->token_type = SUBSHELL;
	subshell_token->next = NULL;
	subshell_token->prev = NULL;
	subshell_node->token = subshell_token;
	subshell_node->left = content;
	subshell_node->right = NULL;
	subshell_node->arguments = NULL;
	return (subshell_node);
}

t_ast	*handle_bracket_content(t_dll **tokens, t_shell *shell)
{
	t_ast	*bracket_content;
	t_ast	*subshell_node;

	*tokens = (*tokens)->next;
	bracket_content = parse_logical_operators(tokens, shell);
	if (*tokens && (*tokens)->bracket)
		*tokens = (*tokens)->next;
	subshell_node = create_subshell_node(bracket_content, shell);
	return (subshell_node);
}
