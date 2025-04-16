/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:31:58 by iammar            #+#    #+#             */
/*   Updated: 2025/04/16 10:27:20 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"
// for testing -------------------------------------------------
void execute_external(t_shell *shell, t_ast *cmd_node)
{
    pid_t pid;
    int status;
    char *cmd;
    cmd = cmd_node->token->value;
    
    char *args[] = {cmd, NULL};
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        shell->exit_code = 1;
        return;
    }
    else if (pid == 0)
    {
        
        if (execvp(cmd, args) == -1)
        {
            perror(cmd);
            exit(127);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->exit_code = 128 + WTERMSIG(status);
    }
}