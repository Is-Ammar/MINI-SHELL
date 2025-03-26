/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/26 01:58:14 by habdella         ###   ########.fr       */
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
		{
			ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (ft_strdup(dir));
	}
	else if (ft_strcmp(dir_type, "OLDPWD") == 0)
	{
		dir = get_env_var(*env, "OLDPWD");
		if (!dir)
		{
			if (!cwd)
				return (NULL);
			return (ft_strdup(cwd));
		}
		ft_putstr_fd(dir, 1);
		ft_putstr_fd("\n", 1);
		return (ft_strdup(dir));
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
			if (cwd)
			{
				ft_putstr_fd(cwd, 1);
				ft_putstr_fd("\n", 1);
				return (ft_strdup(cwd));
			}
			ft_putstr_fd("Minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		return (get_directory(env, "OLDPWD", cwd));
	}
	return (ft_strdup(arg_token->value));
}

int	parse_cd_args(t_dll *arg_token, t_env **env, char **dir, char **cwd)
{
	*dir = resolve_directory(arg_token, env, *cwd);
	if (!*dir)
		return (1);
	if (arg_token && arg_token->next && arg_token->next->token_type == WORD)
	{
		ft_putstr_fd("Minishell: cd: too many arguments\n", 2);
		free(*dir);
		*dir = NULL;
		return (1);
	}
	return (0);
}

void	update_pwd_vars(t_env **env, char *cwd)
{
	char	*new_cwd;

	if (cwd)
		set_env_var(env, "OLDPWD", cwd);
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
	{
		perror("Minishell: cd: ");
		return;
	}
	set_env_var(env, "PWD", new_cwd);
}

void	execute_builtin_cd(t_dll *tokens, t_env **env)
{
	char	*dir;
	t_dll	*arg_token;
	char	*cwd;

	dir = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd: ");
		return;
	}
	arg_token = tokens->next;
	if (parse_cd_args(arg_token, env, &dir, &cwd))
	{
		free(cwd);
		return;
	}
	if (chdir(dir) != 0)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		perror(dir);
		free(dir);
		free(cwd);
		return;
	}
	update_pwd_vars(env, cwd);
}
