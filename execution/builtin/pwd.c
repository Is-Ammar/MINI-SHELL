/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/24 21:46:39 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void execute_builtin_pwd(t_env *env)
{
    char *cwd;
    
    cwd = get_env_var(env, "PWD");
    if (!cwd || access(cwd, F_OK) == -1)
    {
        cwd = getcwd(NULL, 0);
        if (!cwd)
        {
            perror("pwd");
            exit(1);
        }
    }
    printf("%s\n", cwd);
    free(cwd);
}
