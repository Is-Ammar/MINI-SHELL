/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:05:33 by iammar            #+#    #+#             */
/*   Updated: 2025/06/15 16:57:11 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	redirection(t_shell *shell, t_ast *redir)
{
	int	redirect_result;

	redirect_result = 0;
	while (redir && redir->token && redir->token->redir_type != 0)
	{
		if (redir->token->redir_type == READ)
			redirect_result = in_fd(shell, &redir->token, redir->token);
		else if (redir->token->redir_type == WRITE)
			redirect_result = out_fd(shell, &redir->token, redir->token,
					O_TRUNC);
		else if (redir->token->redir_type == APPEND)
			redirect_result = out_fd(shell, &redir->token, redir->token,
					O_APPEND);
		if (redirect_result)
		{
			shell->exit_code = 1;
			break ;
		}
		redir = redir->right;
	}
	return (redirect_result);
}

void	execute_redirections(t_shell *shell, t_ast *original_ast)
{
	int		saved_stdout;
	int		saved_stdin;
	int		redirect_result;
	t_ast	*redir;
	t_ast	*command;

	redirect_result = 0;
	save_restore_fds(&saved_stdout, &saved_stdin, 0);
	redir = shell->ast;
	command = redir->left;
	redirect_result = redirection(shell, redir);
	if (!redirect_result)
	{
		shell->ast = command;
		execute_ast(shell);
	}
	save_restore_fds(&saved_stdout, &saved_stdin, 1);
	shell->ast = original_ast;
}
