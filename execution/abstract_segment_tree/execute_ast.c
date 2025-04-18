/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/04/18 15:43:48 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void execute_ast(t_shell *shell)
{
    if (!shell->ast->left && !shell->ast->right)
    {
        if (shell->ast->token && shell->ast->token->token_type == WORD)
        {
            if (is_builtin(shell))
                execute_builtin(shell);
            else
                execute_external(shell);
        }
        return;
    }
    
    if (shell->ast->token)
    {
        t_ast *original_ast = shell->ast;
        int exit_status = 0;

        if (shell->ast->token->operator == AND)
        {
            shell->ast = original_ast->left;
            execute_ast(shell);
            exit_status = shell->exit_code;

            if (exit_status == 0)
            {
                shell->ast = original_ast->right;
                execute_ast(shell);
            }
        }
        else if (shell->ast->token->operator == OR)
        {

            shell->ast = original_ast->left;
            execute_ast(shell);
            exit_status = shell->exit_code;
            if (exit_status != 0)
            {
                shell->ast = original_ast->right;
                execute_ast(shell);
            }
        }

        else if (shell->ast->token->token_type == PIPE)
        {
            printf("pipe");//(execute_pipe(shell))
        }
        
        shell->ast = original_ast;
    }
}
