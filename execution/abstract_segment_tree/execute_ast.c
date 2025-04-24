/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/04/24 18:11:27 by iammar           ###   ########.fr       */
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

void execute_simple_command(t_shell *shell)
{
    int saved_stdout;
    int saved_stdin;
    t_dll *curr;
    t_dll *next;
    
    if (shell->ast->token && (shell->ast->token->token_type == WORD || shell->ast->token->token_type == REDIRECTION))
    {
        saved_stdout = dup(STDOUT_FILENO);
        saved_stdin = dup(STDIN_FILENO);

        if (shell->ast->token)
            expansion(&shell->ast->token, shell->ast->token, shell->env_list, shell->exit_code);
            

        curr = shell->ast->arguments;
        while (curr)
        {
            next = curr->next;
            
            if(expansion(&shell->ast->arguments, curr, shell->env_list, shell->exit_code))
                printf("dhjksfghsjf");
            curr = next;
        }
        if (redirections(&shell->ast->token) == 0 && 
            redirections(&shell->ast->arguments) == 0)
        {
            if (shell->ast->token && shell->ast->token->token_type == WORD)
            {
                if (is_builtin(shell))
                    execute_builtin(shell);
                else
                    execute_external(shell);
            }
        }
        else
        {
            shell->exit_code = 1;
        }
        
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
    }
}

void execute_ast(t_shell *shell)
{
    t_ast *original_ast;
    int exit_status;
    
    if (shell->ast->token && shell->ast->token->inside_parentheses == TRUE &&
        !shell->subshell)
    {
        shell->subshell++;
        execute_subshell(shell);
        shell->subshell--;
        return;
    }
    
    if (!shell->ast->left && !shell->ast->right)
    {
        execute_simple_command(shell);
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