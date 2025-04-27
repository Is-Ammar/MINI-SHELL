/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:39:54 by iammar            #+#    #+#             */
/*   Updated: 2025/04/27 12:03:00 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static int is_executable(const char *path)
{
    return (access(path, X_OK) == 0);
}

static char	*handle_absolute_path(t_shell *shell, char *cmd)
{
	struct stat	sb;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &sb) == 0 && is_executable(cmd))
			return (ft_strdup(shell, cmd));
	}
	return (NULL);
}

static char	*search_in_path(t_shell *shell, char *cmd, char **paths)
{
	int		i;
	char	*full_path;
	char	*temp;

	i = 0;
	while (paths && paths[i])
	{
		temp = ft_strjoin(shell, "/", cmd);
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(shell, paths[i], temp);
		free(temp);
		if (full_path && is_executable(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
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
	free_split(paths);
	return (result);
}
