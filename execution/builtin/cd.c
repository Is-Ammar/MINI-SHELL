/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/18 15:43:28 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

char	*get_directory(t_shell *shell, char *dir_type, char *cwd)
{
	char	*dir;

	if (ft_strcmp(dir_type, "HOME") == 0)
	{
		dir = get_env_var(shell, shell->env_list, "HOME");
		if (!dir)
		{
			ft_printf("Minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (dir);
	}
	else if (ft_strcmp(dir_type, "OLDPWD") == 0)
	{
		dir = get_env_var(shell, shell->env_list, "OLDPWD");
		if (!dir)
		{
			if (!cwd)
				return (NULL);
			return (ft_strdup(shell, cwd));
		}
		printf("%s\n", dir);
		return (dir);
	}
	return (NULL);
}

char	*resolve_directory(t_dll *arg_token, t_shell *shell, char *cwd)
{
	if (!arg_token || !arg_token->value || ft_strcmp(arg_token->value, "") == 0)
		return (get_directory(shell, "HOME", NULL));
	if (ft_strcmp(arg_token->value, "-") == 0)
	{
		if (!get_env_var(shell, shell->env_list, "OLDPWD"))
		{
			if (cwd)
			{
				printf("%s\n", cwd);
				return (ft_strdup(shell, cwd));
			}
			ft_printf("Minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		return (get_directory(shell, "OLDPWD", cwd));
	}
	return (ft_strdup(shell, arg_token->value));
}

int	parse_cd_args(t_dll *arg_token, t_shell *shell, char **dir, char **cwd)
{
	*dir = resolve_directory(arg_token, shell, *cwd);
	if (!*dir)
		return (1);
	if (arg_token && arg_token->next && arg_token->next)
	{
		ft_printf("Minishell: cd: too many arguments\n");
		*dir = NULL;
		return (1);
	}
	return (0);
}

void	update_pwd_vars(t_shell *shell, char *old_cwd)
{
	char	*new_cwd;
	char	*pwd;

	if (old_cwd)
		set_env_var(shell, &shell->env_list, "OLDPWD", old_cwd);
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
	{
		if (errno == ENOENT)
		{
			pwd = get_env_var(shell, shell->env_list, "PWD");
			if (pwd)
				set_env_var(shell, &shell->env_list, "PWD", pwd);
		}
		else
		{
			perror("Minishell: cd: ");
			shell->exit_code = 1;
		}
		return ;
	}
	set_env_var(shell, &shell->env_list, "PWD", new_cwd);
	free(new_cwd);
}

void	execute_builtin_cd(t_shell *shell)
{
	char	*dir;
	t_dll	*arg_token;
	char	*cwd;
	char *tmp;

	dir = NULL;
	cwd = getcwd(NULL, 0);
	tmp = ft_strdup(shell, cwd);
	free(cwd);
	if (!tmp)
	{
		if (errno == ENOENT)
		{
			tmp = get_env_var(shell, shell->env_list, "PWD");
		}
	}
	if (!tmp)
	{
		perror("Minishell: cd: ");
		shell->exit_code = 1;
		return ;
	}
	arg_token = shell->ast->arguments;
	if (parse_cd_args(arg_token, shell, &dir, &tmp))
	{
		shell->exit_code = 1;
		return ;
	}
	if (chdir(dir) != 0)
	{
		ft_printf("Minishell: cd: ");
		perror(dir);
		shell->exit_code = 1;
		return ;
	}
	update_pwd_vars(shell, tmp);
	shell->exit_code = 0;
}
