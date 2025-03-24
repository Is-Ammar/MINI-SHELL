/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/23 19:59:47 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"
//HANDLE EXIT CODES !!!!!// 
char	*get_directory(t_env **env, char *dir_type, char *cwd)
{
	char	*dir;

	if (ft_strcmp(dir_type, "HOME") == 0)
	{
		dir = get_env_var(*env, "HOME");
		if (!dir)
			ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		return (dir);
	}
	else if (ft_strcmp(dir_type, "OLDPWD") == 0)
	{
		dir = get_env_var(*env, "OLDPWD");
		if (!dir)
			return (cwd);
		ft_putstr_fd(dir, 1);
		ft_putstr_fd("\n", 1);
		return (dir);
	}
	return (NULL);
}

char	*resolve_directory(t_dll *arg_token, t_env **env, char *cwd)
{
	if (!arg_token || arg_token->token_type != WORD || !arg_token->value
		|| ft_strcmp(arg_token->value, "~") == 0
		|| ft_strcmp(arg_token->value, "") == 0)
		return (get_directory(env, "HOME", NULL));
	if (ft_strcmp(arg_token->value, "-") == 0)
	{
		if (!get_env_var(*env, "OLDPWD"))
		{
			ft_putstr_fd(cwd, 1);
			ft_putstr_fd("\n", 1);
			return (cwd);
		}
		return (get_directory(env, "OLDPWD", cwd));
	}
	return (arg_token->value);
}

int	parse_cd_args(t_dll *arg_token, t_env **env, char **dir, char **cwd)
{
	*dir = resolve_directory(arg_token, env, *cwd);
	if (!*dir)
		return (1);
	if (arg_token && arg_token->next && arg_token->next->token_type == WORD)
	{
		ft_putstr_fd("Minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

void	update_pwd_vars(t_env **env, char **cwd)
{
	char	*new_cwd;

	set_env_var(env, "OLDPWD", *cwd);
	free(*cwd);
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
	{
		perror("Minishell: cd: ");
		return ;
	}
	set_env_var(env, "PWD", new_cwd);
	free(new_cwd);
	*cwd = NULL;
}
void	execute_builtin_cd(t_dll *tokens, t_env **env)
{
	char	*dir;
	t_dll	*arg_token;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd: ");
		return ;
	}
	arg_token = tokens->next;
	if (parse_cd_args(arg_token, env, &dir, &cwd))
	{
		free(cwd);
		return ;
	}
	if (chdir(dir) != 0)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		perror(dir);
		free(cwd);
		return ;
	}
	update_pwd_vars(env, &cwd);
}
