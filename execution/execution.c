/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/04 08:20:23 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../smash.h"

int is_builtin(t_shell *shell)
{
    char    *cmd;

    remove_quotes_expand(&shell->tokens, &shell->env_list, shell->exit_code);
    cmd = (shell->tokens)->value;
    if (!cmd)
        return (0);
    return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
        || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
        || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
        || ft_strcmp(cmd, "exit") == 0);
}

void execute_builtin(t_shell *shell)
{
    if (!shell->tokens || !shell->tokens->value)
        return;
    
    if (ft_strcmp(shell->tokens->value, "cd") == 0)
        execute_builtin_cd(shell);
    else if (ft_strcmp(shell->tokens->value, "pwd") == 0)
        execute_builtin_pwd(shell);
    else if (ft_strcmp(shell->tokens->value, "echo") == 0)
        execute_builtin_echo(shell);
    else if (ft_strcmp(shell->tokens->value, "export") == 0)
        execute_builtin_export(shell);
    else if (ft_strcmp(shell->tokens->value, "unset") == 0)
     execute_builtin_unset(shell);
    else if (ft_strcmp(shell->tokens->value, "env") == 0)
        execute_builtin_env(shell);
    else if (ft_strcmp(shell->tokens->value, "exit") == 0)
        execute_builtin_exit(shell);
}

void execution(t_shell *shell)
{
    if (!shell->tokens)
        return;
    if (!shell->tokens->value)
        return;
    // t_env *tmp = shell->env_list;
    // while(tmp)
    // {
    //     printf("----name:%s\n",tmp->env_name);
    //     printf("----value:%s\n\n\n",tmp->env_value);
    //     tmp = tmp->next;
    // }
    // printf("-\n\n\n\n\n\n\n\n\n\n\n\n\n");
    if (is_builtin(shell))
        execute_builtin(shell);
    // else
    //  execute_external_command(shell);
}
