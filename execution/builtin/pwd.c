/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 22:52:21 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"
//not finished yet
void execute_builtin_pwd(t_shell *shell)
{
    char *cwd;
    
    cwd = ft_strdup(get_env_var(shell->env_list, "PWD"));
    if (!cwd || access(cwd, F_OK) == -1)
    {
        cwd = get_current_dir_safe(shell);
        if (!cwd)
        {
            shell->exit_code = 1;// addd error message////////////
            exit(1);
        }
    }
    printf("%s\n", cwd);
    free(cwd);
    shell->exit_code = 0;
}
