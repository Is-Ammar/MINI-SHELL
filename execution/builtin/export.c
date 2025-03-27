/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:17:37 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 21:13:55 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_export(t_shell *shell)
{
    char    *name;
    char    *value;
    char    *tmp;
    t_dll   *current;
    int     exit_code;
    
    exit_code = 0;
    current = shell->tokens->next;
    
    if (!current)
        return;
    
    while (current)
    {
        tmp = ft_strdup(current->value);
        if (!tmp)
            return;
        
        value = ft_strchr(tmp, '=');
        if (value)
        {
            *value = '\0';
            name = tmp;
            value++;
            if (is_valid_identifier(name))
                set_env_var(&shell->env_list, name, value);
            else
            {
                ft_putstr_fd("export: '", 2);
                ft_putstr_fd(current->value, 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                exit_code = 1;
            }
        }
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
    
    shell->exit_code = exit_code;
}
