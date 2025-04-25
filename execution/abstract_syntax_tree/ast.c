/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:22 by iammar            #+#    #+#             */
/*   Updated: 2025/04/25 13:15:32 by iammar           ###   ########.fr       */
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
        
        logical_node = malloc(sizeof(t_ast));
        if (!logical_node)
            return NULL;
        
        logical_node->token = *tokens;
        logical_node->left = result;
        
        *tokens = (*tokens)->next;
        logical_node->right = parse_pipe(tokens, shell);
        if (!logical_node->right)
        {
            free_ast(logical_node);
            return NULL;
        }
        
        result = logical_node;
    }
    
    return result;
}

t_ast *parse_pipe(t_dll **tokens, t_shell *shell)
{
    t_ast *result;
	t_ast *pipe_node;
    
    result  = parse_simple_command(tokens, shell);
    while (*tokens && (*tokens)->value && (*tokens)->token_type == PIPE)
    {
		
        pipe_node = malloc(sizeof(t_ast));
        if (!pipe_node)
            return NULL;
        
        pipe_node->token = *tokens;
        pipe_node->left = result;
        
        *tokens = (*tokens)->next;
        pipe_node->right = parse_simple_command(tokens, shell);
        if (!pipe_node->right)
        {
            free_ast(pipe_node);
            return NULL;
        }
        
        result = pipe_node;
    }
    
    return result;
}

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
