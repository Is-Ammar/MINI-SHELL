/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/16 23:27:12 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_echo(t_shell *shell)
{
    t_arg   *curr;
    int     flag;
    int     i;

    i = 1;
    if (!shell->ast || !shell->ast->token || !shell->ast->arguments)
    {
        printf("\n");
        return;
    }
    
    (1) && (flag = 0, curr = shell->ast->arguments);

    if (curr && ft_strchr(curr->argument, '-'))
    {
        while(curr->argument[i] == 'n')
        {
            i++;
        }
        flag = 1;
        curr = curr->next;
    }

    while (curr)
    {
        printf("%s", curr->argument);
        curr = curr->next;
        if (curr)
            printf(" ");
    }

    if (!flag)
        printf("\n");
        
    shell->exit_code = 0;
}