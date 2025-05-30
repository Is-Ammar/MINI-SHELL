/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_settings.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/05/29 13:09:52 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_env	*create_env_var(t_shell *shell, char *name, char *value)
{
	t_env	*new_var;

	new_var = ft_malloc(shell, sizeof(t_env));
	new_var->env_name = ft_strdup(shell, name);
	new_var->env_value = ft_strdup(shell, value);
	new_var->next = NULL;
	return (new_var);
}

void	set_env_var(t_shell *shell, t_env **env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*new_var;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->env_name, name) == 0)
		{
			current->env_value = ft_strdup(shell, value);
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

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->env_name, name) == 0)
			return (ft_strdup(shell, current->env_value));
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
