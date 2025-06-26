/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:31:58 by iammar            #+#    #+#             */
/*   Updated: 2025/06/26 09:30:00 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	child_exit_code(pid_t pid, int status, int exit_code)
{
	waitpid(pid, &status, 0);
	exit_code = get_exit_code(status);
	return (exit_code);
}

int	execute(t_shell *shell, char *path, char *cmd, char **args)
{
	pid_t	pid;
	char	**env;
	int		status;
	int		exit_code;

	env = convert_env_to_array(shell, shell->env_list);
	signal(SIGINT, SIG_IGN);
	status = 0;
	exit_code = 0;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		reset_signal_handlers();
		if (execve(path, args, env) == -1)
			execve_errors(shell, cmd);
		clean_exit(shell, shell->exit_code);
	}
	else
		exit_code = child_exit_code(pid, status, exit_code);
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

static char	**prepare_command_args(t_shell *shell, char *cmd, t_dll *args_list,
		int ac)
{
	char	**args;
	t_dll	*tmp;
	int		i;

	args = ft_malloc(shell, (ac + 2) * sizeof(char *), 0);
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

void	execute_external(t_shell *shell)
{
	char	*cmd;
	char	**args;
	char	*path;
	int		ac;

	path = NULL;
	cmd = ft_strdup(shell, shell->ast->token->value);
	ac = count_args(shell->ast->arguments);
	args = prepare_command_args(shell, cmd, shell->ast->arguments, ac);
	if (!args)
	{
		shell->exit_code = 1;
		return ;
	}
	if (check_valid_cmd(shell, cmd))
		return ;
	path = get_command_path(shell, cmd, shell->env_list);
	if (path)
	{
		if (shell->exit_code == 0 && !ac)
			set_env_var(shell, &shell->env_list, "_", path);
		else
			set_env_var(shell, &shell->env_list, "_", args[ac]);
		shell->exit_code = execute(shell, path, cmd, args);
	}
}
