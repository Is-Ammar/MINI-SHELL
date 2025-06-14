/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/14 14:03:47 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void check_argument(t_shell *shell)
{
    char *arg;
    struct stat file_stat;
    
    if (shell->ast->token && shell->ast->arguments)
    {
        arg = shell->ast->arguments->value;
        if (stat(arg, &file_stat) == 0)
        {
            if (opendir(arg) || (access(arg, X_OK) == -1))
            {
                ft_printf("env: %s: Permission denied\n", arg);
                shell->exit_code = 126;
                return;
            }
        }
        else
        {
            ft_printf("env: %s: No such file or directory\n", arg);
            shell->exit_code = 127;
            return;
        }
        shell->exit_code = 1;
        return;
    }
}

void execute_builtin_env(t_shell *shell)
{
    t_env *tmp;

    if (!shell->tokens || !shell->env_list)
        return;
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
