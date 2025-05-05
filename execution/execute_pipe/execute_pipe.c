/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:19:38 by iammar            #+#    #+#             */
/*   Updated: 2025/05/05 15:29:39 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int execute_pipe(t_shell *shell)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int status;
    t_ast *original_ast;
    
    original_ast = shell->ast;
    
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        return 1;
    }

    pid1 = fork();
    if (pid1 == -1) 
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    
    if (pid1 == 0) 
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        shell->ast = original_ast->left;
        execute_ast(shell);
        exit(shell->exit_code);
    }
    
    pid2 = fork();
    if (pid2 == -1) 
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        kill(pid1, SIGTERM);
        waitpid(pid1, NULL, 0);
        return 1;
    }
    
    if (pid2 == 0) 
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        shell->ast = original_ast->right;
        execute_ast(shell);
        exit(shell->exit_code);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    shell->exit_code = WEXITSTATUS(status);
    
    shell->ast = original_ast;
    return shell->exit_code;
}
