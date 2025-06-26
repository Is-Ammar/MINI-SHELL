/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 19:07:47 by iammar            #+#    #+#             */
/*   Updated: 2025/06/26 08:38:56 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	get_exit_code(int status)
{
	int	code;

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			writing(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			writing(STDERR_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGSEGV)
			writing(STDERR_FILENO, "Segmentation fault (core dumped)\n", 33);
		else if (WTERMSIG(status) == SIGKILL)
			writing(STDERR_FILENO, "Killed\n", 7);
		code = 128 + WTERMSIG(status);
	}
	else if (WIFSTOPPED(status))
		code = 128 + WSTOPSIG(status);
	else
		code = WEXITSTATUS(status);
	return (code);
}

void	execute_subshell(t_shell *shell)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		if (shell->savedin > 2)
			close(shell->savedin);
		if (shell->savedout > 2)
			close(shell->savedout);
		reset_signal_handlers();
		shell->ast = shell->ast->left;
		execute_ast(shell);
		clean_exit(shell, shell->exit_code);
	}
	else
	{
		waitpid(pid, &status, 0);
		shell->exit_code = get_exit_code(status);
	}
}
