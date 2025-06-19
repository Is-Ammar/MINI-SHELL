/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:39:11 by iammar            #+#    #+#             */
/*   Updated: 2025/06/19 13:55:48 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static void	execute_and_operator(t_shell *shell, t_ast *original_ast)
{
	shell->ast = original_ast->left;
	execute_ast(shell);
	if (shell->exit_code == 0)
	{
		shell->ast = original_ast->right;
		execute_ast(shell);
	}
}

static void	execute_or_operator(t_shell *shell, t_ast *original_ast)
{
	shell->ast = original_ast->left;
	execute_ast(shell);
	if ((shell->exit_code != 0 && shell->exit_code < 128)
		|| shell->exit_code == 131)
	{
		shell->ast = original_ast->right;
		execute_ast(shell);
	}
}

static void	handle_operator(t_shell *shell, t_ast *original_ast)
{
	if (shell->ast->token->operator == AND)
		execute_and_operator(shell, original_ast);
	else if (shell->ast->token->operator == OR)
		execute_or_operator(shell, original_ast);
	else if (shell->ast->token->token_type == PIPE)
		shell->exit_code = execute_pipe(shell);
}
void	execute_ast(t_shell *shell)
{
	t_ast	*original_ast;

	if (shell->ast->token && shell->ast->token->token_type == SUBSHELL)
	{
		execute_subshell(shell);
		return ;
	}
	if (shell->ast->token && shell->ast->token->redir_type != 0)
	{
		original_ast = shell->ast;
		execute_redirections(shell, original_ast);
		return ;
	}
	if (!shell->ast->left && !shell->ast->right)
	{
		execute_simple_command(shell);
		return ;
	}
	if (shell->ast->token)
	{
		original_ast = shell->ast;
		handle_operator(shell, original_ast);
		shell->ast = original_ast;
	}
}
