/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:39:54 by iammar            #+#    #+#             */
/*   Updated: 2025/06/14 16:47:53 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static char	*handle_absolute_path(t_shell *shell, char *cmd)
{
	struct stat	sb;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &sb) == 0 && access(cmd, X_OK) == 0)
			return (ft_strdup(shell, cmd));
	}
	return (NULL);
}

static char	*search_in_path(t_shell *shell, char *cmd, char **paths)
{
	struct stat	st;
	int			i;
	char		*full_path;
	char		*temp;

	i = 0;
	while (paths && paths[i])
	{
		temp = ft_strjoin(shell, "/", cmd);
		full_path = ft_strjoin(shell, paths[i], temp);
		if (stat(full_path, &st) == 0)
		{
			if (full_path && access(full_path, X_OK) == 0
				&& !S_ISDIR(st.st_mode))
				return (full_path);
		}
		i++;
	}
	return (NULL);
}

int	check_valid_cmd(t_shell *shell, char *cmd)
{
	struct stat	stats;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &stats) != 0)
		{
			shell->exit_code = exec_error(shell, cmd, EDIRFILE);
			return (1);
		}
		if (S_ISDIR(stats.st_mode))
		{
			ft_printf(B_WHITE "minishell: %s Is a directory\n" RESET, cmd);
			return (shell->exit_code = 126, 1);
		}
	}
	if (cmd[0] == '.' && cmd[1] && cmd[1] == '/')
	{
		if (stat(cmd, &stats) == 0 && S_ISREG(stats.st_mode) && access(cmd,
				X_OK) != 0)
		{
			exec_error(shell, cmd, EPERMISS);
			return (shell->exit_code = 126, 1);
		}
	}
	return (0);
}

char	*get_command_path(t_shell *shell, char *cmd, t_env *env_list)
{
	char	*path_value;
	char	**paths;
	char	*result;

	result = handle_absolute_path(shell, cmd);
	if (result)
		return (result);
	path_value = get_env_var(shell, env_list, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(shell, path_value, ':');
	result = search_in_path(shell, cmd, paths);
	if (!result)
		shell->exit_code = exec_error(shell, cmd, ECOMMAND);
	return (result);
}
