/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 00:04:01 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 00:04:51 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

char	*get_current_dir(t_shell *shell)
{
	char	*cwd;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	tmp = ft_strdup(shell, cwd);
	free(cwd);
	if (!tmp && errno == ENOENT)
		tmp = get_env_var(shell, shell->env_list, "PWD");
	return (tmp);
}

int	handle_cd_error(char *dir)
{
	ft_printf("Minishell: cd: ");
	perror(dir);
	return (1);
}

int	process_cd_change(t_shell *shell, char *dir, char *tmp)
{
	if (chdir(dir) != 0)
		return (handle_cd_error(dir));
	update_pwd_vars(shell, tmp);
	return (0);
}

void	execute_builtin_cd(t_shell *shell)
{
	char	*dir;
	char	*tmp;

	dir = NULL;
	tmp = get_current_dir(shell);
	if (!tmp)
	{
		perror("Minishell: cd: ");
		shell->exit_code = 1;
		return ;
	}
	if (parse_cd_args(shell->ast->arguments, shell, &dir, &tmp))
	{
		shell->exit_code = 1;
		return ;
	}
	shell->exit_code = process_cd_change(shell, dir, tmp);
}
