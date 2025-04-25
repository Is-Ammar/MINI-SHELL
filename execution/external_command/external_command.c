/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:31:58 by iammar            #+#    #+#             */
/*   Updated: 2025/04/25 13:42:22 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	execute(char *cmd, char *path, char **args, char **env)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	status = 0;
	exit_code = 0;
	(void)cmd;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (execve(path, args, env) == -1)
			perror("execve: ");
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
	}
	return (exit_code);
}


static int	count_args(t_dll *args_list)
{
	t_dll	*tmp;
	int		ac;

	ac = 0;
	tmp = args_list;
	while (tmp)
	{
		ac++;
		tmp = tmp->next;
	}
	return (ac);
}

static char	**prepare_command_args(char *cmd, t_dll *args_list, int ac)
{
	char	**args;
	t_dll	*tmp;
	int		i;

	args = malloc((ac + 2) * sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = cmd;
	i = 1;
	tmp = args_list;
	while (tmp)
	{
		args[i++] = tmp->value;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

static void	handle_command_not_found(t_shell *shell, char *cmd)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	shell->exit_code = 127;
}

void	execute_external(t_shell *shell)
{
	char	*cmd;
	char	**args;
	char	**env;
	char	*path;
	int		ac;

	cmd = shell->ast->token->value;
	ac = count_args(shell->ast->arguments);
	args = prepare_command_args(cmd, shell->ast->arguments, ac);
	if (!args)
	{
		shell->exit_code = 1;
		return ;
	}
	env = convert_env_to_array(shell->env_list);
	path = get_command_path(cmd, shell->env_list);
	if (path)
	{
		shell->exit_code = execute(cmd, path, args, env);
		free(path);
	}
	else
		handle_command_not_found(shell, cmd);
	free(args);
	free_split(env);
}