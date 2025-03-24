/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/24 20:57:36 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

double	ft_atol(char *str)
{
	double	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str++ - '0');
	}
	return (sign * result);
}
void execute_builtin_exit(t_dll *tokens)
{
    int status;
    long long_status;
    char *arg;
    char *ptr;
    
    printf("exit\n");
    status = 0;
    
    if (tokens->next && tokens->next->value)
    {
        arg = tokens->next->value;

        if (*arg == '\0')
        {
            ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
            exit(2);
        }
        ptr = arg;
        if (*ptr == '+' || *ptr == '-')
            ptr++;
        int valid = 1;
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
 
        long_status = ft_atol(arg);
  
        if (tokens->next->next && tokens->next->next->value)
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            return; // will return the exit status !!!!!!!!!!!!!!!!!
        }
        else
        {

           
            status = (int) long_status;
        }
    }
    
    exit(status);
}
