/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:31:58 by iammar            #+#    #+#             */
/*   Updated: 2025/05/30 08:59:22 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	execute(t_shell *shell, char *cmd, char *path, char **args)
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
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		reset_signal_handlers();
		if (!*cmd)
		{
			exec_error(shell, "''", ECOMMAND);
			clean_exit(shell, 127);
		}
		if (cmd[0] == '.' && cmd[1] && cmd[1] != '/')
		{
			exec_error(shell, cmd, ECOMMAND);
			clean_exit(shell, 0);
		}
		if (ft_strchr(cmd, '/'))
		{
			execve(cmd, args, env);
			exec_error(shell, cmd, EDIRFILE);
		}
		else
		{
			execve(path, args, env);
			exec_error(shell, cmd, ECOMMAND);
		}
		clean_exit(shell, 0);
	}
	else
	{
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_IGN);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
        {
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
        if (WIFSIGNALED(status))
            exit_code = 128 + WTERMSIG(status);
        else
            exit_code = WEXITSTATUS(status);
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

static char	**prepare_command_args(t_shell *shell, char *cmd, t_dll *args_list, int ac)
{
	char	**args;
	t_dll	*tmp;
	int		i;

	args = ft_malloc(shell, (ac + 2) * sizeof(char *));
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

	cmd = ft_strdup(shell, shell->ast->token->value);
	ac = count_args(shell->ast->arguments);
	args = prepare_command_args(shell, cmd, shell->ast->arguments, ac);
	if (!args)
	{
		shell->exit_code = 1;
		return ;
	}
	path = get_command_path(shell, cmd, shell->env_list);
	if (path || cmd)
	{
		shell->exit_code = execute(shell, cmd, path, args);
		if(shell->exit_code == 0 && !ac)
			set_env_var(shell, &shell->env_list,"_",path);
		else
			set_env_var(shell, &shell->env_list,"_", args[ac]);
	}
	else
		shell->exit_code = exec_error(shell, cmd, ECOMMAND);
}
