/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 19:07:47 by iammar            #+#    #+#             */
/*   Updated: 2025/06/16 13:05:39 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	execute_subshell(t_shell *shell)
{
	pid_t	pid;
	int		status;

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
