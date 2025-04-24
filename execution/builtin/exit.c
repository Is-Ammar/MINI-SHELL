/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/24 20:28:04 by iammar           ###   ########.fr       */
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
    t_dll *arg_token;
    
    printf("exit\n");
    status = shell->exit_code;
    if (shell->ast && shell->ast->token && shell->ast->arguments)
    {
        arg_token = shell->ast->arguments;
        arg = arg_token->value;

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

        if (arg_token->next && arg_token->next->value)
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