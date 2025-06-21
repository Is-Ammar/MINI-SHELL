/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/21 22:48:22 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	update_pwd_vars(t_shell *shell, char *dir)
{
	if(!ft_strcmp(dir, "..") || !ft_strcmp(dir, "."))
	{
		shell->pwd = ft_strjoin(shell, shell->pwd, "/");
		shell->pwd = ft_strjoin(shell, shell->pwd, dir);
	}
	set_env_var(shell, &shell->env_list, "PWD", shell->pwd);
}

int	process_cd_change(t_shell *shell, char *dir)
{
	char	*tmp;
	
	if (chdir(dir) != 0)
	{
		update_pwd_vars(shell, dir);
		ft_putstr_fd("Minishell: cd: ", 2);
		perror(dir);
		return (1);
	}
	tmp = getcwd(NULL, 0);
	if (!tmp)
		;
	else
		shell->pwd = tmp;
	set_env_var(shell, &shell->env_list, "PWD", shell->pwd);
	return (0);
}
