/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:19:38 by iammar            #+#    #+#             */
/*   Updated: 2025/06/06 13:46:18 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int execute_pipe(t_shell *shell)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int status;
    int exit_code;
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
        clean_exit(shell, shell->exit_code);
    }
    
    pid2 = fork();
    if (pid2 == -1) 
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 2);
        return 1;
    }
    
    if (pid2 == 0) 
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        shell->ast = original_ast->right;
        execute_ast(shell);
        clean_exit(shell, shell->exit_code);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 2);
    waitpid(pid2, &status, 2);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
    {
        write(STDERR_FILENO, "Quit (core dumped)\n", 19);
    }
    if (WIFSIGNALED(status))
        exit_code = 128 + WTERMSIG(status);
    else
        exit_code = WEXITSTATUS(status);
    shell->exit_code = exit_code;
    
    shell->ast = original_ast;
    return shell->exit_code;
}