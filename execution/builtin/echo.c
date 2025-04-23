/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/23 08:25:54 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_echo(t_shell *shell)
{
    t_dll   *curr;
    int     flag;
    int     i;

    if (!shell->ast || !shell->ast->token || !shell->ast->arguments)
    {
        printf("\n");
        return;
    }
    flag = 0;
    curr = shell->ast->arguments;
    if (curr && curr->value[0] == '-' && curr->value[1] == 'n')
    {
        i = 2;
        while (curr->value[i] == 'n')
            i++;
        if (curr->value[i] == '\0')
        {
            flag = 1;
            curr = curr->next;
        }
    }
    while (curr)
    {
        printf("%s", curr->value);
        if (curr)
            printf(" ");
        curr = curr->next;
    }
    if (!flag)
        printf("\n");
    shell->exit_code = 0;
}
