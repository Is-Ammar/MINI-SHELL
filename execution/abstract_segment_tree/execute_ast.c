/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/04/22 15:15:14 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_subshell(t_shell *shell)
{
    pid_t   pid;
    int     status;
    
    pid = fork();
    if (pid == 0) 
    {
        execute_ast(shell);
        exit(0);
    } 
    else 
    {
        waitpid(pid,&status, 0);
    }
    shell->exit_code = WEXITSTATUS(status);
}

void execute_ast(t_shell *shell)
{
    t_ast *original_ast;
    int exit_status;
    t_dll   *curr;
    
    if (shell->ast->token && shell->ast->token->inside_parentheses == TRUE &&
        shell->subshell)
    {
        shell->subshell++;
        execute_subshell(shell);
        shell->subshell--;
        return;
    }
    
    if (!shell->ast->left && !shell->ast->right)
    {
        if (shell->ast->token && shell->ast->token->token_type == WORD)
        {
            expansion(&shell->ast->token, shell->env_list, shell->exit_code);
            curr = shell->ast->arguments;
            while(curr)
            {
                expansion(&shell->ast->arguments, shell->env_list, shell->exit_code);
                curr = curr->next;
            }
            if (is_builtin(shell))
                execute_builtin(shell);
            else
                execute_external(shell);
        }
        return;
    }
    
    if (shell->ast->token)
    {
        original_ast = shell->ast;
        exit_status = 0;

        if (shell->ast->token->operator == AND)
        {
            shell->ast = original_ast->left;
            execute_ast(shell);
            exit_status = shell->exit_code;

            if (exit_status == 0)
            {
                shell->ast = original_ast->right;
                execute_ast(shell);
            }
        }
        else if (shell->ast->token->operator == OR)
        {
            shell->ast = original_ast->left;
            execute_ast(shell);
            exit_status = shell->exit_code;
            
            if (exit_status != 0)
            {
                shell->ast = original_ast->right;
                execute_ast(shell);
            }
        }
        else if (shell->ast->token->token_type == PIPE)
        {
           exit_status = execute_pipe(shell);
        }
        
        shell->ast = original_ast;
    }
}