/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:22 by iammar            #+#    #+#             */
/*   Updated: 2025/05/13 19:53:37 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_ast *abstract_segment_tree(t_shell *shell)
{
    t_ast *abst = NULL;
    t_dll *curr_token;
    
    curr_token = shell->tokens;
    if (!curr_token || !curr_token->value)
        return NULL;
    
    abst = parse_logical_operators(&curr_token, shell);
    return abst;
}

t_ast *parse_logical_operators(t_dll **tokens, t_shell *shell)
{
    t_ast *result;
	t_ast *logical_node;
    
    result = parse_pipe(tokens, shell);
    while (*tokens && (*tokens)->value && (*tokens)->token_type == OPERATOR)
    {
        
        logical_node = ft_malloc(shell, sizeof(t_ast));
        if (!logical_node)
            return NULL;
        logical_node->token = *tokens;
        logical_node->left = result;
        *tokens = (*tokens)->next;
        logical_node->right = parse_pipe(tokens, shell);
        if (!logical_node->right)
            return NULL;
        result = logical_node;
    }
    
    return result;
}

t_ast *parse_pipe(t_dll **tokens, t_shell *shell)
{
    t_ast *result;
	t_ast *pipe_node;
    
    result  = parse_redirections(tokens, shell);
    while (*tokens && (*tokens)->value && (*tokens)->token_type == PIPE)
    {
		
        pipe_node = ft_malloc(shell, sizeof(t_ast));
        if (!pipe_node)
            return NULL;
        pipe_node->token = *tokens;
        pipe_node->left = result;
        *tokens = (*tokens)->next;
        pipe_node->right = parse_redirections(tokens, shell);
        if (!pipe_node->right)
            return NULL;
        result = pipe_node;
    }
    return result;
}

t_ast *parse_redirections(t_dll **tokens, t_shell *shell)
{
    t_ast *result;
    t_ast *redir_node;
    t_ast *command_node = NULL;
    t_ast *prev_redir;
    
    result = parse_simple_command(tokens, shell);
    command_node = result;
    
    while (*tokens && (*tokens)->value && (*tokens)->redir_type != 0)
    {
        redir_node = ft_malloc(shell, sizeof(t_ast));
        redir_node->token = *tokens;
        redir_node->left = command_node;
        redir_node->right = NULL;
        redir_node->arguments = NULL;
        *tokens = (*tokens)->next;
        if (result != command_node)
        {
            prev_redir = result;
            while (prev_redir->right)
                prev_redir = prev_redir->right;
            prev_redir->right = redir_node;
        }
        else
        {
            result = redir_node;
        }
    }
    
    return result;
}
