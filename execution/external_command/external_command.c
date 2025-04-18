/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:31:58 by iammar            #+#    #+#             */
/*   Updated: 2025/04/18 15:51:56 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"
//still needs work
int is_executable(const char *path)
{
    return (access(path, X_OK) == 0);
}

char *get_command_path(char *cmd, t_env *env_list)
{
    struct stat sb;
    char *path_value;
    char **paths;
    int i;
    char *full_path;
    char *temp;

    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (stat(cmd, &sb) == 0 && is_executable(cmd))
            return (ft_strdup(cmd));
        return NULL;
    }
    path_value = get_env_var(env_list, "PATH");
    if (!path_value)
        return NULL;
    paths = ft_split(path_value, ':');
    i = 0;
    full_path = NULL;
    while (paths && paths[i])
    {
        temp = ft_strjoin("/", cmd);
        if (temp)
        {
            full_path = ft_strjoin(paths[i], temp);
            free(temp);
            if (full_path && is_executable(full_path))
            {
                free_split(paths);
                return (full_path);
            }
            free(full_path);
        }
        i++;
    }
    free_split(paths);
    return NULL;
}

int execute(char *cmd, char *path,char **args, char **env)
{
    pid_t pid;
    int status = 0;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        status = 1;
        return status;
    }
    else if (pid == 0)
    {
        
        if (execve(path, args, env) == -1)
        {
            ft_putstr_fd("minishell: '", 2);
            ft_putstr_fd(cmd, 2);
            ft_putstr_fd("': command not found\n", 2);
            exit(127);
        }
    }
     else
    {
        waitpid(pid, &status, 0);
    }
    return status;
    
}


void execute_external(t_shell *shell)
{
    char *cmd;
    t_arg *tmp;
    t_arg *args_list;
    char **args;
    char **env;
    char *path;
    
    int ac = 0;
    int i = 0;
    cmd = shell->ast->token->value;
    

    tmp = shell->ast->arguments;
    args_list = tmp;
    while (tmp)
    {
        ac++;
        tmp = tmp->next;
    }
    
    args = malloc((ac + 2) * sizeof(char *));
    if (!args)
    {
        shell->exit_code = 1;
        return;
    }
    
    args[0] = cmd;
    i = 1;
    tmp = args_list;
    while (tmp)
    {
        args[i++] = tmp->argument;
        tmp = tmp->next;
    }
    args[i] = NULL;
    
    env = convert_env_to_array(shell->env_list);
    path = get_command_path(cmd, shell->env_list);
    
    if (path)
    {
        shell->exit_code = execute(cmd, path, args, env);
        free(path);
    }
    else
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(cmd, 2);
        ft_putstr_fd("\n", 2);
        shell->exit_code = 127;
    }
    
    free(args);
    free_split(env);
}
