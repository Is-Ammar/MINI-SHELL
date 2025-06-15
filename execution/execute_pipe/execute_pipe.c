/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:19:38 by iammar            #+#    #+#             */
/*   Updated: 2025/06/15 14:18:41 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static int	setup_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static int	wait_and_get_exit_code(pid_t pid1, pid_t pid2)
{
	int	status;
	int	exit_code;

	waitpid(pid1, NULL, 2);
	waitpid(pid2, &status, 2);
	signal(SIGINT, SIG_IGN);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	else
		exit_code = WEXITSTATUS(status);
	return (exit_code);
}

static int	fork_first_child(t_shell *shell, int pipefd[2], pid_t *pid1,
		t_ast *ast)
{
	*pid1 = fork();
	if (*pid1 == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (*pid1 == 0)
		handle_child1(shell, pipefd, ast);
	return (0);
}

static int	fork_second_child(t_shell *shell, int pipefd[2], pid_t *pid2,
		t_ast *ast)
{
	*pid2 = fork();
	if (*pid2 == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (*pid2 == 0)
		handle_child2(shell, pipefd, ast);
	return (0);
}

int	execute_pipe(t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	t_ast	*original_ast;

	original_ast = shell->ast;
	signal(SIGINT, SIG_IGN);
	if (setup_pipe(pipefd) == 1)
		return (1);
	if (fork_first_child(shell, pipefd, &pid1, original_ast) == 1)
		return (1);
	if (fork_second_child(shell, pipefd, &pid2, original_ast) == 1)
	{
		waitpid(pid1, NULL, 2);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	shell->ast = original_ast;
	return (wait_and_get_exit_code(pid1, pid2));
}
