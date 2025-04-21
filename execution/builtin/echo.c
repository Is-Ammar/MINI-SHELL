/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/19 13:13:24 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_echo(t_shell *shell)
{
    t_arg   *curr;
    int     flag;
    int     i;

    if (!shell->ast || !shell->ast->token || !shell->ast->arguments)
    {
        printf("\n");
        return;
    }
    
    flag = 0;
    curr = shell->ast->arguments;
    if (curr && curr->argument[0] == '-' && curr->argument[1] == 'n')
    {
        i = 2;
        while (curr->argument[i] == 'n')
            i++;
        if (curr->argument[i] == '\0')
        {
            flag = 1;
            curr = curr->next;
        }
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
