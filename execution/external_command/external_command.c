/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:31:58 by iammar            #+#    #+#             */
/*   Updated: 2025/05/09 15:36:04 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	execute(char *cmd, char *path, char **args, char **env)
{
	pid_t	pid;
	int		status;
	int		exit_code;

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
		if (ft_strchr(cmd, '/'))
			execve(cmd, args, env);
		else
			execve(path, args, env);
		ft_error(cmd, ECOMMAND);
		exit(0);
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
	char	**env;
	char	*path;
	int		ac;

	cmd = shell->ast->token->value;
	ac = count_args(shell->ast->arguments);
	args = prepare_command_args(shell, cmd, shell->ast->arguments, ac);
	if (!args)
	{
		shell->exit_code = 1;
		return ;
	}
	env = convert_env_to_array(shell, shell->env_list);
	path = get_command_path(shell, cmd, shell->env_list);
	if (path)
	{
		shell->exit_code = execute(cmd, path, args, env);
		if(shell->exit_code == 0 && !ac)
		{
			set_env_var(shell, &shell->env_list,"_",path);
		}
		else
		set_env_var(shell, &shell->env_list,"_", args[ac]);
	}
	else
		shell->exit_code = ft_error(cmd, ECOMMAND);
	free_split(env);
}
