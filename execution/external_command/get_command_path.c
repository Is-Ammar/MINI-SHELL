/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:39:54 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 10:48:09 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static char	*search_in_path(t_shell *shell, char *cmd, char **paths)
{
	int			i;
	char		*full_path;
	char		*temp;
	int			flag;

	i = 0;
	flag = 0;
	while (paths && paths[i])
	{
		temp = ft_strjoin(shell, "/", cmd);
		full_path = ft_strjoin(shell, paths[i], temp);
		if (full_path && access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == -1)
				flag = 1;
			else if (!check_valid_path(full_path))
				return (full_path);
		}
		i++;
	}
	if (flag)
		return (shell->exit_code = 126, \
			exec_error(shell, cmd, EPERMISS), NULL);
	shell->exit_code = exec_error(shell, cmd, ECOMMAND);
	return (NULL);
}

static char	*handle_absolute_path(t_shell *shell, char *cmd)
{
	struct stat	sb;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &sb) == 0 && access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == -1)
			{
				exec_error(shell, cmd, EPERMISS);
				return (shell->exit_code = 126, NULL);
			}
			else
				return (ft_strdup(shell, cmd));
		}
	}
	return (NULL);
}

char	*get_command_path(t_shell *shell, char *cmd, t_env *env_list)
{
	char	*path_value;
	char	**paths;
	char	*result;

	shell->exit_code = 0;
	result = handle_absolute_path(shell, cmd);
	if (shell->exit_code && !result)
		return (result);
	if (result)
		return (result);
	path_value = get_env_var(shell, env_list, "PATH");
	if (!path_value)
		return (shell->exit_code = exec_error(shell, cmd, EDIRFILE), NULL);
	paths = ft_split(shell, path_value, ':');
	result = search_in_path(shell, cmd, paths);
	return (result);
}
