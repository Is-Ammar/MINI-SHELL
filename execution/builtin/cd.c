/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/22 21:54:38 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void handle_cd_flag(t_dll *arg_token, t_env **env,  char **dir, char **cwd)
{
    
    if (!arg_token || arg_token->token_type != WORD || !arg_token->value || 
        ft_strcmp(arg_token->value, "~") == 0 || ft_strcmp(arg_token->value, "") == 0)
    {
        *dir = get_env_var(*env, "HOME");
        if (!dir || !*dir)
        {
            ft_putstr_fd("cd: HOME not set\n", 2);
            free(cwd);
            return;
        }
    }
    else if (ft_strcmp(arg_token->value, "-") == 0)
    {
        *dir = get_env_var(*env, "OLDPWD");
        if (!dir || !*dir)
        {
            dir = cwd;
            ft_putstr_fd(*dir, 1);
            ft_putstr_fd("\n", 1);
        }
        else
        {
            ft_putstr_fd(*dir, 1);
            ft_putstr_fd("\n", 1);
        }
    }
    else
    {
        *dir = arg_token->value;

        if (arg_token->next && arg_token->next->token_type == WORD)
        {
            ft_putstr_fd("cd: too many arguments\n", 2);
            free(cwd);
            return;
        }
    }
}
void execute_builtin_cd(t_dll *tokens, t_env **env)
{
    char    *dir;
    t_dll   *arg_token;
    char    *cwd;
    

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        perror("cd: ");
        return;
    }
    arg_token = tokens->next;
    
    handle_cd_flag(arg_token, env, &dir, &cwd);
    if (chdir(dir) != 0)
    {
        ft_putstr_fd("Minishell: cd: ", 2);
        perror(dir);
        free(cwd);
        return;
    }
    set_env_var(env, "OLDPWD", cwd);
    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        perror("Minishell: cd: ");
    }
    else
    {
        set_env_var(env, "PWD", cwd);
        free(cwd);
    }
}