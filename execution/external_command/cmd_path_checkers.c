/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:47:42 by habdella          #+#    #+#             */
/*   Updated: 2025/06/20 10:48:31 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	check_absolute_cmd(t_shell *shell, char *cmd)
{
	struct stat	stats;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &stats) == -1)
		{
			if (errno == ENOENT)
			{
				shell->exit_code = exec_error(shell, cmd, EDIRFILE);
				return (1);
			}
			if (errno == ENOTDIR)
			{
				exec_error(shell, cmd, ENOTDIR);
				return (shell->exit_code = 126, 1);
			}
			return (perror("minishell: "), 1);
		}
		if (S_ISDIR(stats.st_mode))
		{
			exec_error(shell, cmd, EISDIR);
			return (shell->exit_code = 126, 1);
		}
	}
	return (0);
}

int	check_valid_cmd(t_shell *shell, char *cmd)
{
	struct stat	stats;
	int			ret_val;

	ret_val = check_absolute_cmd(shell, cmd);
	if (ret_val)
		return (ret_val);
	if (cmd[0] == '.' && cmd[1] && cmd[1] == '/')
	{
		if (stat(cmd, &stats) == 0 && S_ISREG(stats.st_mode)
			&& access(cmd, X_OK) != 0)
		{
			exec_error(shell, cmd, EPERMISS);
			return (shell->exit_code = 126, 1);
		}
	}
	return (0);
}

int	check_valid_path(char *full_path)
{
	struct stat	stats;

	if (stat(full_path, &stats) == -1)
	{
		return (1);
	}
	if (S_ISDIR(stats.st_mode))
	{
		return (1);
	}
	return (0);
}
