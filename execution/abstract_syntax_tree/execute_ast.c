/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/04/25 10:34:43 by iammar           ###   ########.fr       */
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
        exit(shell->exit_code);
    } 
    else 
    {
        waitpid(pid,&status, 0);
    }
    shell->exit_code = WEXITSTATUS(status);
}

static void	execute_and_operator(t_shell *shell, t_ast *original_ast)
{
	shell->ast = original_ast->left;
	execute_ast(shell);
	if (shell->exit_code == 0)
	{
		shell->ast = original_ast->right;
		execute_ast(shell);
	}
}

static void	execute_or_operator(t_shell *shell, t_ast *original_ast)
{
	shell->ast = original_ast->left;
	execute_ast(shell);
	if (shell->exit_code != 0)
	{
		shell->ast = original_ast->right;
		execute_ast(shell);
	}
}

static void	handle_operator(t_shell *shell, t_ast *original_ast)
{
	if (shell->ast->token->operator == AND)
		execute_and_operator(shell, original_ast);
	else if (shell->ast->token->operator == OR)
		execute_or_operator(shell, original_ast);
	else if (shell->ast->token->token_type == PIPE)
		shell->exit_code = execute_pipe(shell);
}

void	execute_ast(t_shell *shell)
{
	t_ast	*original_ast;

	if (shell->ast->token && shell->ast->token->inside_parentheses == TRUE
		&& !shell->subshell)
	{
		shell->subshell++;
		execute_subshell(shell);
		shell->subshell--;
		return ;
	}
	if (!shell->ast->left && !shell->ast->right)
	{
		execute_simple_command(shell);
		return ;
	}
	if (shell->ast->token)
	{
		original_ast = shell->ast;
		handle_operator(shell, original_ast);
		shell->ast = original_ast;
	}
}
