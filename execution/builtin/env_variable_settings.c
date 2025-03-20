/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_settings.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/20 21:17:49 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

char	*ft_strdup(const char *str)
{
	int		i;
	char	*dup;

	if (*str == '\0')
		return (NULL);
	dup = (char *)malloc((ft_strlen(str) + 1) * sizeof(char));
	if (dup == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (str[i] != '\0')
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

// size_t	ft_strlen(char *str)
// {
// 	size_t	i;

// 	if (str == NULL)
// 		return (0);
// 	i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }

t_env *create_env_var(const char *name, const char *value)
{
    t_env *new_var = malloc(sizeof(t_env));
    if (!new_var)
        return NULL;

    new_var->env_name = ft_strdup(name);
    new_var->env_value = ft_strdup(value);
    new_var->next = NULL;

    return new_var;
}

void set_env_var(t_env **env_list, char *name,char *value)
{
    t_env *current = *env_list;

    while (current)
    {
        if (ft_strcmp(current->env_name, name) == 0)
        {
            free(current->env_value);
            current->env_value = ft_strdup(value);
            return;
        }
        current = current->next;
    }

    t_env *new_var = create_env_var(name, value);
    new_var->next = *env_list;
    *env_list = new_var;
}

const char *get_env_var(t_env *env_list, char *name)
{
    t_env *current = env_list;

    while (current)
    {
        if (ft_strcmp(current->env_name, name) == 0)
            return current->env_value;
        current = current->next;
    }

    return NULL;
}

void free_env_list(t_env *env_list)
{
    t_env *current = env_list;
    t_env *next;

    while (current)
    {
        next = current->next;
        free(current->env_name);
        free(current->env_value);
        free(current);
        current = next;
    }
}


