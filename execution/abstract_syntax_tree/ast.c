/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:22 by iammar            #+#    #+#             */
/*   Updated: 2025/06/06 08:24:51 by habdella         ###   ########.fr       */
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
        
        logical_node = ft_malloc(shell, sizeof(t_ast), 0);
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
		
        pipe_node = ft_malloc(shell, sizeof(t_ast), 0);
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
    t_ast *result = NULL;
    t_ast *command_node = NULL;
    t_ast *first_redir = NULL;
    t_ast *last_redir = NULL;
    t_ast *redir_node;
    t_ast *curr_redir;
    
    while (*tokens && (*tokens)->token_type == REDIRECTION)
    {
        redir_node = ft_malloc(shell, sizeof(t_ast), 0);
        redir_node->token = *tokens;
        redir_node->left = NULL;
        redir_node->right = NULL;
        redir_node->arguments = NULL;
        
        if (!first_redir)
            first_redir = redir_node;
        if (last_redir)
            last_redir->right = redir_node;
        last_redir = redir_node;
        
        *tokens = (*tokens)->next;
    }
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
        
        if (last_redir)
            last_redir->right = redir_node;
        else
            result = redir_node;
        last_redir = redir_node;
        
        *tokens = (*tokens)->next;
    }
    
    return result;
}