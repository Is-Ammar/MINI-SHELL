/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/22 13:08:57 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_env(t_shell *shell)
{
    t_env *tmp;

    if (!shell->tokens || !shell->env_list)
        return;
    if (shell->ast->token && shell->ast->arguments)
    {
        printf("env: '%s': No such file or directory\n", shell->ast->arguments->value);
        shell->exit_code = 1;
        return;
    }
    tmp = shell->env_list;
    while (tmp)
    {
        printf("%s=", tmp->env_name);
        if (tmp->env_value)
            printf("%s", tmp->env_value);
        printf("\n");
        tmp = tmp->next;
    }
    shell->exit_code = 0;
}
