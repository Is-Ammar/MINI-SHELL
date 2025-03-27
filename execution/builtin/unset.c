/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:56:08 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 21:11:05 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_unset(t_shell *shell)
{
    t_dll *current;
    
    if (!shell->tokens->next)
    {
        shell->exit_code = 0;
        return;
    }
    
    current = shell->tokens->next;
    while (current)
    {
        if (is_valid_identifier(current->value))
            unset_env_var(shell, current);
        else
        {
            ft_putstr_fd("unset: '", 2);
            ft_putstr_fd(current->value, 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            shell->exit_code = 1;
        }
        current = current->next;
    }
    
    shell->exit_code = 0;
}
