/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:17:37 by iammar            #+#    #+#             */
/*   Updated: 2025/03/26 21:17:23 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

#include <ctype.h>

int	is_valid_identifier(const char *str)
{
	int	i;
	 
	if (!str || !*str)
		return (0);
		
	if (!isalpha(str[0]) && str[0] != '_')
		return (0);
		
	i = 1;
	while (str[i])
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	
	return (1);
}

int	execute_builtin_export(t_dll *tokens, t_env **env)
{
	char	*name;
	char	*value;
	char	*tmp;
	t_dll	*current;
	int		exit_code;
	
	exit_code = 0;
	current = tokens->next;
	
	if (!current)
		return (0);
	
	while (current)
	{
		tmp = ft_strdup(current->value);
		if (!tmp)
			return (1);
		
		value = ft_strchr(tmp, '=');
		if (value)
		{
			*value = '\0';
			name = tmp;
			value++;
			if (is_valid_identifier(name))
				set_env_var(env, name, value);
			else
			{
				ft_putstr_fd("export: '", 2);
				ft_putstr_fd(current->value, 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				exit_code = 1;
			}
		}
		// else if (is_valid_identifier(tmp))
		// {
		// 	mark_env_var_exported(env, tmp);
		// }
		else
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(current->value, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_code = 1;
		}
		
		free(tmp);
		current = current->next;
	}
	
	return (exit_code);
}