/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/29 22:05:55 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_builtin_env(t_shell *shell)
{
    t_env *tmp;
    struct stat file_stat;
    char *arg;

    if (!shell->tokens || !shell->env_list)
        return;

    if (shell->ast->token && shell->ast->arguments)
    {
        arg = shell->ast->arguments->value;

        if (stat(arg, &file_stat) == 0)
        {
            if (opendir(arg))
            {
                ft_putstr_fd("env: ", 2);
                ft_putstr_fd(arg, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                shell->exit_code = 126;
                return;
            }
            if (access(arg, X_OK) == -1)
            {
                ft_putstr_fd("env: ", 2);
                ft_putstr_fd(arg, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                shell->exit_code = 126;
                return;
            }
        }
        else
        {
            ft_putstr_fd("env: ", 2);
            ft_putstr_fd(arg, 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            shell->exit_code = 127;
            return;
        }
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
