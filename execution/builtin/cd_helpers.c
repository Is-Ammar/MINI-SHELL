/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/22 01:24:15 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	update_pwd_vars(t_shell *shell, char *dir)
{
	char	*new_pwd;

	ft_putstr_fd("cd: error retrieving current directory: \
		No such file or directory\n", 2);
	if (!ft_strcmp(dir, ".."))
	{
		new_pwd = ft_strjoin(shell, shell->pwd, "/..");
		shell->pwd = env_strdup(shell, new_pwd);
	}
	else if (!ft_strcmp(dir, "."))
	{
		new_pwd = ft_strjoin(shell, shell->pwd, "/.");
		shell->pwd = env_strdup(shell, new_pwd);
	}
	else
	{
		shell->pwd = env_strdup(shell, dir);
	}
}

int	process_cd_change(t_shell *shell, char *dir)
{
	char	*tmp;
	char	*tmp2;
	if (chdir(dir) != 0)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		perror(dir);
		return (1);
	}
	tmp = getcwd(NULL, 0);
	tmp2 = ft_strdup(shell, tmp);
	free(tmp);
	if (!tmp2)
	{
		update_pwd_vars(shell, dir);
	}
	else
	{
		shell->pwd = tmp2;
	}
	set_env_var(shell, &shell->env_list, "PWD", shell->pwd);
	return (0);
}
