/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_settings.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 00:51:44 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_env	*create_env_var(t_shell *shell, char *name, char *value)
{
	t_env	*new_var;

	new_var = ft_malloc(shell, sizeof(t_env), 1);
	new_var->env_name = env_strdup(shell, name);
	new_var->env_value = env_strdup(shell, value);
	new_var->next = NULL;
	return (new_var);
}

void	set_env_var(t_shell *shell, t_env **env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*new_var;

	if (!name)
		return ;
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->env_name, name) == 0)
		{
			current->env_value = env_strdup(shell, value);
			return ;
		}
		current = current->next;
	}
	new_var = create_env_var(shell, name, value);
	new_var->next = *env_list;
	*env_list = new_var;
}

char	*get_env_var(t_shell *shell, t_env *env_list, char *name)
{
	t_env	*current;

	if (!name)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (current && ft_strcmp(current->env_name, name) == 0)
			return (env_strdup(shell, current->env_value));
		current = current->next;
	}
	return (NULL);
}

void	add_env_var(t_shell *shell, t_env **env_list, char *name, char *value)
{
	t_env	*new_var;
	t_env	*current;

	new_var = create_env_var(shell, name, value);
	if (!new_var)
		return ;
	if (!*env_list)
	{
		*env_list = new_var;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_var;
	return ;
}

void	update_shlvl(t_shell *shell, t_env **env)
{
	char	*shlvl_entry;
	int		shlvl_value;
	char	*new_shlvl;

	shlvl_entry = NULL;
	shlvl_value = 0;
	new_shlvl = NULL;
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
			set_env_var(shell, env, env_strdup(shell, "SHLVL"), new_shlvl);
	}
}
