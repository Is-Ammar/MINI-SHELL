/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/23 08:59:09 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../smash.h"

int is_builtin(t_shell *shell)
{
    char    *cmd;
    
    cmd = shell->ast->token->value;
    if (!cmd)
        return (0);
    return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
        || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
        || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
        || ft_strcmp(cmd, "exit") == 0);
}

void execute_builtin(t_shell *shell)
{
    if (!shell->ast || !shell->ast->token)
        return;
    
    if (ft_strcmp(shell->ast->token->value, "cd") == 0)
        execute_builtin_cd(shell);
    else if (ft_strcmp(shell->ast->token->value, "pwd") == 0)
        execute_builtin_pwd(shell);
    else if (ft_strcmp(shell->ast->token->value, "echo") == 0)
        execute_builtin_echo(shell);
    else if (ft_strcmp(shell->ast->token->value, "export") == 0)
        execute_builtin_export(shell);
    else if (ft_strcmp(shell->ast->token->value, "unset") == 0)
     execute_builtin_unset(shell);
    else if (ft_strcmp(shell->ast->token->value, "env") == 0)
        execute_builtin_env(shell);
    else if (ft_strcmp(shell->ast->token->value, "exit") == 0)
        execute_builtin_exit(shell);
}

void execution(t_shell *shell)
{
    if (!shell->ast)
        return;
    if (!shell->ast->token || !shell->ast->token->value)
        return;
    execute_ast(shell);
}
