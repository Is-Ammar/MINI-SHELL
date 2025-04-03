/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/03 14:23:35 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_echo(t_shell *shell)
{
    t_dll   *curr;
    int     flag;

    if (!shell->tokens->next || (shell->tokens->next && shell->tokens->next->token_type != WORD))
    {
        printf("\n");
        return;
    }
    (1) && (flag = 0, curr = shell->tokens->next);
    while (!ft_strcmp(curr->value, "-n"))
    {
        flag = 1;
        curr = curr->next;
    }
    while (curr && curr->token_type == WORD)
    {
        printf("%s", curr->value);
        curr = curr->next;
        if (curr && curr->token_type == WORD)
            printf(" ");
    }
    if (!flag)
        printf("\n");
    shell->exit_code = 0;
}
