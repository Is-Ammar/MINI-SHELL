/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/25 21:03:16 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void execute_builtin_pwd(t_env *env)
{
    char *cwd;
    
    cwd = ft_strdup(get_env_var(env, "PWD"));
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
