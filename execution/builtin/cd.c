/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 22:51:39 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

char *get_directory(t_shell *shell, char *dir_type, char *cwd)
{
    char *dir;

    if (ft_strcmp(dir_type, "HOME") == 0)
    {
        dir = get_env_var(shell->env_list, "HOME");
        if (!dir)
        {
            ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
            return (NULL);
        }
        return (dir);
    }
    else if (ft_strcmp(dir_type, "OLDPWD") == 0)
    {
        dir = get_env_var(shell->env_list, "OLDPWD");
        if (!dir)
        {
            if (!cwd)
                return (NULL);
            return (ft_strdup(cwd));
        }
        ft_putstr_fd(dir, 1);
        ft_putstr_fd("\n", 1);
        return (dir);
    }
    return (NULL);
}

char *resolve_directory(t_dll *arg_token, t_shell *shell, char *cwd)
{
    if (!arg_token || arg_token->token_type != WORD || !arg_token->value
        || ft_strcmp(arg_token->value, "~") == 0
        || ft_strcmp(arg_token->value, "") == 0)
        return (get_directory(shell, "HOME", NULL));
    if (ft_strcmp(arg_token->value, "-") == 0)
    {
        if (!get_env_var(shell->env_list, "OLDPWD"))
        {
            if (cwd)
            {
                ft_putstr_fd(cwd, 1);
                ft_putstr_fd("\n", 1);
                return (ft_strdup(cwd));
            }
            ft_putstr_fd("Minishell: cd: OLDPWD not set\n", 2);
            return (NULL);
        }
        return (get_directory(shell, "OLDPWD", cwd));
    }
    return (ft_strdup(arg_token->value));
}

int parse_cd_args(t_dll *arg_token, t_shell *shell, char **dir, char **cwd)
{
    *dir = resolve_directory(arg_token, shell, *cwd);
    if (!*dir)
        return (1);
    if (arg_token && arg_token->next && arg_token->next->token_type == WORD)
    {
        ft_putstr_fd("Minishell: cd: too many arguments\n", 2);
        free(*dir);
        *dir = NULL;
        return (1);
    }
    return (0);
}

void update_pwd_vars(t_shell *shell, char *cwd)
{
    char *new_cwd;

    if (cwd)
        set_env_var(&shell->env_list, "OLDPWD", cwd);
    new_cwd = getcwd(NULL, 0);
    if (!new_cwd)
    {
        perror("Minishell: cd: ");
        shell->exit_code = 1;
        return;
    }
    set_env_var(&shell->env_list, "PWD", new_cwd);
}

void execute_builtin_cd(t_shell *shell)
{
    char *dir;
    t_dll *arg_token;
    char *cwd;

    dir = NULL;
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("Minishell: cd: ");
        shell->exit_code = 1;
        return;
    }
    arg_token = shell->tokens->next;
    if (parse_cd_args(arg_token, shell, &dir, &cwd))
    {
        free(cwd);
        shell->exit_code = 1;
        return;
    }
    if (chdir(dir) != 0)
    {
        ft_putstr_fd("Minishell: cd: ", 2);
        perror(dir);
        free(dir);
        free(cwd);
        shell->exit_code = 1;
        return;
    }
    update_pwd_vars(shell, cwd);
    shell->exit_code = 0;
}
