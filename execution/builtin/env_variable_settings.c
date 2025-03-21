/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_settings.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/21 22:59:42 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

t_env *create_env_var(const char *name, const char *value)
{
    t_env *new_var;
    
    new_var  = malloc(sizeof(t_env));
    if (!new_var)
        return NULL;

    new_var->env_name = ft_strdup(name);
    new_var->env_value = ft_strdup(value);
    new_var->next = NULL;

    return new_var;
}

void set_env_var(t_env **env_list, char *name,char *value)
{
    t_env *current;
    t_env *new_var;

    current = *env_list;
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

     new_var = create_env_var(name, value);
    new_var->next = *env_list;
    *env_list = new_var;
}

char *get_env_var(t_env *env_list, char *name)
{
    t_env *current;
    current = env_list;

    while (current)
    {
        if (ft_strcmp(current->env_name, name) == 0)
            return current->env_value;
        current = current->next;
    }

    return NULL;
}

void add_env_var(t_env **env_list, const char *name, const char *value)
{
    t_env *new_var;
    t_env *current;
    new_var = create_env_var(name, value);
    if (!new_var)
        return;
    if (!*env_list)
    {
        *env_list = new_var;
        return;
    }
    current = *env_list;
    while (current->next)
        current = current->next;
    
    current->next = new_var;
    return;
}

void free_env_list(t_env *env_list)
{
    t_env *current;
    
    current = env_list;
    while (current)
    {
        free(current->env_name);
        free(current->env_value);
        free(current);
        current = current->next;;
    }
}
