/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:03:27 by iammar            #+#    #+#             */
/*   Updated: 2025/04/12 14:11:22 by iammar           ###   ########.fr       */
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

t_ast *parse_command_sequence(t_dll **tokens, t_shell *shell)
{
    t_ast *result;
    t_ast *redir_node;
    
    result = parse_simple_command(tokens, shell);
    if (!result)
        return NULL;

    while (*tokens && (*tokens)->value)
    {
        if (!((*tokens)->direction || (*tokens)->heredoc))
            break;
        
        redir_node = malloc(sizeof(t_ast));
        if (!redir_node)
            return NULL;
        
        redir_node->token = *tokens;
        redir_node->left = result;
        
        *tokens = (*tokens)->next;

        if (!*tokens || !(*tokens)->value)
        {
            free_ast(redir_node);
            return NULL;
        }
        
        redir_node->right = malloc(sizeof(t_ast));
        if (!redir_node->right)
        {
            free_ast(redir_node);
            return NULL;
        }
        
        redir_node->right->token = *tokens;
        redir_node->right->left = NULL;
        redir_node->right->right = NULL;
        
        *tokens = (*tokens)->next;
        result = redir_node;
    }
    
    return result;
}

t_ast *parse_simple_command(t_dll **tokens, t_shell *shell) 
{
    t_ast *cmd_node;
    t_ast *bracket_content;
    t_arg *new_arg;
    t_arg **tail;
    
    if (!*tokens || !(*tokens)->value)
        return NULL;

    if ((*tokens)->bracket) 
    {
        *tokens = (*tokens)->next;
        bracket_content = parse_logical_operators(tokens, shell);
        if (*tokens && (*tokens)->bracket)
            *tokens = (*tokens)->next;
        return bracket_content;
    }

    cmd_node = malloc(sizeof(t_ast));
    if (!cmd_node)
        return NULL;
    
    cmd_node->token = *tokens;
    cmd_node->arguments = NULL;
    cmd_node->left = NULL;
    cmd_node->right = NULL;

    *tokens = (*tokens)->next;
    tail = &cmd_node->arguments;

    while (*tokens && (*tokens)->value && (*tokens)->token_type == WORD) 
    {
    new_arg = malloc(sizeof(t_arg));
    new_arg->argument = (*tokens)->value;
    new_arg->next = NULL;
    *tail = new_arg;
    tail = &new_arg->next;
    *tokens = (*tokens)->next;
    }
    return cmd_node;
}
