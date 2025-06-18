/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 19:07:47 by iammar            #+#    #+#             */
/*   Updated: 2025/06/18 15:35:18 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int get_exit_code(int status)
{
	int code;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		code = 128 + WTERMSIG(status);
	}
	if (WIFSIGNALED(status))
		code = 128 + WTERMSIG(status);
	else
		code = WEXITSTATUS(status);
	return (code);
}
void	execute_subshell(t_shell *shell)
{
	pid_t	pid;
	int		status;
	pid_t 	wait;

	pid = fork();
	if (pid == 0)
	{
		reset_signal_handlers();
		shell->ast->forked = TRUE;
		shell->ast = shell->ast->left;
		execute_ast(shell);
		clean_exit(shell, shell->exit_code);
	}
	else
	{
		wait = waitpid(pid, &status, 0);
		if(wait == -1 && errno == ECHILD)
			shell->exit_code = 127;
		else 
			shell->exit_code = get_exit_code(status);
	}
}
