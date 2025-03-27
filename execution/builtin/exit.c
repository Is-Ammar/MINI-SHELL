/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 17:39:02 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_exit(t_shell *shell)
{
    int status;
    long long_status;
    char *arg;
    char *ptr;
    int valid;
    
    printf("exit\n");
    status = 0;
    
    if (shell->tokens->next && shell->tokens->next->value)
    {
        arg = shell->tokens->next->value;

        if (*arg == '\0')
        {
            ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
            exit(2);
        }
        ptr = arg;
        if (*ptr == '+' || *ptr == '-')
            ptr++;
        valid = 1;
        while (*ptr != '\0')
        {
            if (*ptr < '0' || *ptr > '9')
            {
                valid = 0;
                break;
            }
            ptr++;
        }
        
        if (!valid)
        {
            ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
            exit(2);
        }
 
        long_status = ft_atoi(arg);
  
        if (shell->tokens->next->next && shell->tokens->next->next->value)
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            shell->exit_code = 1;
            return;
        }
        else
        {
            status = (int) long_status;
        }
    }
    
    exit(status);
}
