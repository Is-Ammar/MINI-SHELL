/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_settings1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/14 20:33:50 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_env	*find_env_var(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->env_name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	create_env(t_shell *shell, t_env **env, char **environ)
{
	int		i;
	char	*equals_sign;
	char	*name;
	char	*value;
	int		name_len;
	char	*shlvl_entry;
	int		shlvl_value;
	char	*new_shlvl;

	i = 0;
	equals_sign = NULL;
	name = NULL;
	value = NULL;
	name_len = 0;
	shlvl_entry = NULL;
	shlvl_value = 0;
	new_shlvl = NULL;
	i = 0;
	*env = NULL;
	while (environ[i])
	{
		equals_sign = ft_strchr(environ[i], '=');
		if (equals_sign)
		{
			name_len = equals_sign - environ[i];
			name = ft_malloc(shell, name_len + 1, 1);
			ft_strlcpy(name, environ[i], name_len + 1);
			name[name_len] = '\0';
			value = env_strdup(shell, equals_sign + 1);
			add_env_var(shell, env, name, value);
		}
		i++;
	}
	shlvl_entry = get_env_var(shell, *env, "SHLVL");
	if (!shlvl_entry)
	{
		set_env_var(shell, env, env_strdup(shell, "SHLVL"), env_strdup(shell,
				"1"));
	}
	else
	{
		shlvl_value = ft_atoi(shlvl_entry);
		if (shlvl_value < 0)
			shlvl_value = 0;
		else if (shlvl_value >= 999)
		{
			ft_printf("minishell: warning: shell level (%d)", ++shlvl_value),
				ft_printf(" too high, resetting to 1\n");
			shlvl_value = 0;
		}
		shlvl_value++;
		new_shlvl = ft_itoa(shell, shlvl_value, 1);
		if (new_shlvl)
		{
			set_env_var(shell, env, env_strdup(shell, "SHLVL"), new_shlvl);
		}
	}
}

void	unset_env_var(t_shell *shell, t_dll *token)
{
	t_env	*env;
	t_env	*prev;

	env = shell->env_list;
	prev = NULL;
	while (env)
	{
		if (ft_strcmp(env->env_name, token->value) == 0)
		{
			if (prev)
				prev->next = env->next;
			else
				shell->env_list = env->next;
			return ;
		}
		prev = env;
		env = env->next;
	}
}

char	*get_current_dir_safe(t_shell *shell)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (errno == ENOENT)
		{
			pwd = get_env_var(shell, shell->env_list, "PWD");
			if (pwd)
				return (ft_strdup(shell, pwd));
		}
		return (NULL);
	}
	return (cwd);
}

char	**convert_env_to_array(t_shell *shell, t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;
	size_t	len;

	count = 0;
	i = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = ft_malloc(shell, (count + 1) * sizeof(char *), 0);
	current = env_list;
	while (current)
	{
		len = ft_strlen(current->env_name) + ft_strlen(current->env_value) + 2;
		env_array[i] = ft_malloc(shell, len, 0);
		ft_strlcpy(env_array[i], current->env_name, ft_strlen(current->env_name)
			+ 1);
		ft_strcat(env_array[i], "=");
		ft_strcat(env_array[i], current->env_value);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
