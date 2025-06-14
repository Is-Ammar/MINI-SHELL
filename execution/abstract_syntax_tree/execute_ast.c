/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/06/14 15:15:41 by iammar           ###   ########.fr       */
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
		reset_signal_handlers();
        shell->ast->forked = TRUE;
        execute_ast(shell);
        clean_exit(shell, shell->exit_code);
    }
    else
    {
        waitpid(pid, &status, 2);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
        {
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
            shell->exit_code = 128 + WTERMSIG(status);
		}
        if (WIFSIGNALED(status))
            shell->exit_code = 128 + WTERMSIG(status);
        else
            shell->exit_code = WEXITSTATUS(status);
    }
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
	if ((shell->exit_code != 0 && shell->exit_code < 128) || shell->exit_code == 131)
	{
		shell->ast = original_ast->right;
		execute_ast(shell);
	}
}

static void handle_operator(t_shell *shell, t_ast *original_ast)
{
    if (shell->ast->token->operator == AND)
        execute_and_operator(shell, original_ast);
    else if (shell->ast->token->operator == OR)
        execute_or_operator(shell, original_ast);
    else if (shell->ast->token->token_type == PIPE)
        shell->exit_code = execute_pipe(shell);
}

void execute_ast(t_shell *shell)
{
    t_ast *original_ast;

    if (!shell->ast)
        return;

    if (shell->ast->token && shell->ast->token->inside_parentheses == TRUE
        && !shell->subshell)
    {
        shell->subshell++;
        execute_subshell(shell);
        shell->subshell--;
        return;
    }
    if (shell->ast->token && shell->ast->token->redir_type != 0)
    {
        original_ast = shell->ast;
        execute_redirections(shell, original_ast);
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
        handle_operator(shell, original_ast);
        shell->ast = original_ast;
    }
}
