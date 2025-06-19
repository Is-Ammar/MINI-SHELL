/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 00:47:33 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	get_and_check_env(t_shell *shell)
{
	char		*cwd;
	struct stat	pwd_stat;
	struct stat	dot_stat;

	cwd = get_env_var(shell, shell->env_list, "PWD");
	if (cwd)
	{
		if (stat(cwd, &pwd_stat) == 0 && stat(".", &dot_stat) == 0)
		{
			if (pwd_stat.st_dev == dot_stat.st_dev
				&& pwd_stat.st_ino == dot_stat.st_ino)
			{
				printf("%s\n", cwd);
				shell->exit_code = 0;
				return (0);
			}
			return (1);
		}
		return (1);
	}
	return (1);
}

void	execute_builtin_pwd(t_shell *shell)
{
	char	*cwd;
	char	*tmp;

	if (!get_and_check_env(shell))
		return ;
	else
	{
		cwd = getcwd(NULL, 0);
		tmp = ft_strdup(shell, cwd);
		free(cwd);
		if (!tmp)
			if (errno == ENOENT)
				tmp = get_env_var(shell, shell->env_list, "PWD");
		if (!tmp)
		{
			printf("pwd: error retrieving current directory: %s\n",
				strerror(errno));
			shell->exit_code = 1;
			return ;
		}
		printf("%s\n", tmp);
		shell->exit_code = 0;
	}
}
