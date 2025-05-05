/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/05/05 15:16:46 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_subshell(t_shell *shell)
{
    pid_t   pid;
    int     status;
    struct sigaction sa_child;
	struct sigaction sa_parent;
	struct sigaction sa_original_int;
	struct sigaction sa_original_quit;

    sigaction(SIGINT, NULL, &sa_original_int);
    sigaction(SIGQUIT, NULL, &sa_original_quit);
    sa_parent.sa_handler = SIG_IGN;
    sigemptyset(&sa_parent.sa_mask);
    sa_parent.sa_flags = 0;
    sigaction(SIGINT, &sa_parent, NULL);
    sigaction(SIGQUIT, &sa_parent, NULL);
    
    pid = fork();
    if (pid == 0) 
    {
        sa_child.sa_handler = SIG_DFL;
        sigemptyset(&sa_child.sa_mask);
        sa_child.sa_flags = 0;
        sigaction(SIGINT, &sa_child, NULL);
        sigaction(SIGQUIT, &sa_child, NULL);
        execute_ast(shell);
        exit(shell->exit_code);
    } 
    else 
    {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
        {
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
        sigaction(SIGINT, &sa_original_int, NULL);
        sigaction(SIGQUIT, &sa_original_quit, NULL);

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
