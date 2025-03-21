/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/21 21:54:11 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
        || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
        || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
        || ft_strcmp(cmd, "exit") == 0);
}

void execute_builtin(t_dll *tokens, t_env **env)
{
    if (!tokens || !tokens->value)
        return;

    if (ft_strcmp(tokens->value, "cd") == 0)
        execute_builtin_cd(tokens, env);
     else if (ft_strcmp(tokens->value, "pwd") == 0)
     execute_builtin_pwd(*env);
     // else if (ft_strcmp(tokens->value, "echo") == 0)
    //  execute_builtin_echo(tokens->value);
    // else if (ft_strcmp(tokens->value, "export") == 0)
    //  execute_builtin_export(tokens->value);
    // else if (ft_strcmp(tokens->value, "unset") == 0)
    //  execute_builtin_unset(tokens->value);
    // else if (ft_strcmp(tokens->value, "env") == 0)
    //  execute_builtin_env(tokens->value);
    // else if (ft_strcmp(tokens->value, "exit") == 0)
    //  execute_builtin_exit(tokens->value);
}

void execute_input(t_dll *tokens, char **environ)
{
	t_env *env = NULL;

    if (!tokens)
        return;
    
    if (!tokens->value)
        return;
    create_env(&env , environ);
    // while(env)
    // {
    //     printf("----name:%s\n",env->env_name);
    //     printf("----value:%s\n\n\n",env->env_value);
    //     env = env->next;
    // }
    if (is_builtin(tokens->value))
        execute_builtin(tokens, &env);
    // else
    //  execute_external_command(tokens, env);
}
