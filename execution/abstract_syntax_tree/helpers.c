/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:17:50 by iammar            #+#    #+#             */
/*   Updated: 2025/04/29 21:29:43 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void set_last_cmd_env(t_shell *shell)
{
    t_dll *current;

    current = shell->ast->arguments;
    if(current)
    {
        while(current)
        {
            if(!current->next)
                break;
            current = current->next;
        }
        set_env_var(shell, &shell->env_list, "_", current->value);
        return;
    }
    set_env_var(shell, &shell->env_list, "_", shell->ast->token->value);
}
