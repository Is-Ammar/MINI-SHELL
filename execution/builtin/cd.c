/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 00:04:01 by iammar            #+#    #+#             */
/*   Updated: 2025/06/21 23:14:00 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

char	*get_home(t_shell *shell)
{
	char	*dir;

	dir = get_env_var(shell, shell->env_list, "HOME");
	if (!dir)
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		return (NULL);
	}
	return (dir);
}

char	*resolve_directory(t_dll *arg_token, t_shell *shell)
{
	if (!arg_token || !arg_token->value)
		return (get_home(shell));
	return (ft_strdup(shell, arg_token->value));
}

int	parse_cd_args(t_dll *arg_token, t_shell *shell, char **dir)
{
	*dir = resolve_directory(arg_token, shell);
	if (!*dir)
		return (1);
	if (arg_token && arg_token->next)
	{
		ft_putstr_fd("Minishell: cd: too many arguments\n", 2);
		*dir = NULL;
		return (1);
	}
	return (0);
}

char	*get_current_dir(t_shell *shell)
{
	char	*cwd;
	char	*tmp;
	char 	*temp;

	cwd = getcwd(NULL, 0);
	tmp = ft_strdup(shell, cwd);
	free(cwd);
	if (!tmp)
	{
		temp = ft_strjoin(shell, \
		"cd: error retrieving current directory: ", strerror(errno));
		ft_putstr_fd(ft_strjoin(shell, temp, "\n"), 2);
	}
	else
		shell->pwd = tmp;
	return (tmp);
}

void	execute_builtin_cd(t_shell *shell)
{
	char	*dir;
	char	*tmp;

	dir = NULL;
	set_env_var(shell, &shell->env_list, "OLDPWD", shell->pwd);
	tmp = get_current_dir(shell);
	if (!tmp)
	{
		set_env_var(shell, &shell->env_list, "PWD", shell->pwd);
		shell->exit_code = 1;
	}
	if (parse_cd_args(shell->ast->arguments, shell, &dir))
	{
		shell->exit_code = 1;
		return ;
	}
	shell->exit_code = process_cd_change(shell, dir);
}
