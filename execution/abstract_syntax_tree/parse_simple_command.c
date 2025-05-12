/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:03:27 by iammar            #+#    #+#             */
/*   Updated: 2025/05/12 16:26:00 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static t_ast *handle_bracket_content(t_dll **tokens, t_shell *shell)
{
    t_ast   *bracket_content;
    t_dll   *current;
    int     bracket_level;
	
	bracket_level = 1;
    *tokens = (*tokens)->next;
    current = *tokens;
    while (current && bracket_level > 0)
    {
        current->inside_parentheses = TRUE;
        if (current->bracket && current->value)
        {
            if (!ft_strcmp(current->value, "("))
                bracket_level++;
            else if (!ft_strcmp(current->value, ")"))
                bracket_level--;
        }
        if (bracket_level)
            current = current->next;
        else
            break;
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

static void	process_command_arguments(t_dll **tokens, t_ast *cmd_node, t_dll **tail)
{
	t_dll	*new_arg;

	while (*tokens && (*tokens)->value && ((*tokens)->token_type == WORD
			|| (*tokens)->redir_type == READ))
	{
		if (cmd_node->token == *tokens)
			;
		else
		{
			new_arg = malloc(sizeof(t_dll));
			if (!new_arg)
				return ;
			copy_token_properties(*tokens, new_arg);
			add_arg_to_list(tail, new_arg);
			tail = &new_arg->next;
		}
		*tokens = (*tokens)->next;
	}
}

t_ast *parse_simple_command(t_dll **tokens, t_shell *shell)
{
    t_ast *cmd_node;
    t_dll **tail;
    t_dll *first_token;
	t_dll *start_tokens;
	
	first_token = *tokens;
    if (!*tokens || !(*tokens)->value)
        return (NULL);
    if ((*tokens)->bracket)
        return (handle_bracket_content(tokens, shell));
    
    cmd_node = ft_malloc(shell, sizeof(t_ast));
    cmd_node->token = find_command(shell, *tokens);
    
    cmd_node->arguments = NULL;
    cmd_node->left = NULL;
    cmd_node->right = NULL;
    tail = &cmd_node->arguments;
    start_tokens = *tokens;
    process_command_arguments(tokens, cmd_node, tail);
    if (!cmd_node->token && start_tokens && (start_tokens->token_type == REDIRECTION))
    {
        cmd_node->token = first_token;
    }
    return (cmd_node);
}
