/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:35:17 by iammar            #+#    #+#             */
/*   Updated: 2025/05/08 18:07:58 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

static void	save_restore_fds(int *saved_stdout, int *saved_stdin, int restore)
{
	if (restore == 0)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		*saved_stdin = dup(STDIN_FILENO);
	}
	else
	{
		dup2(*saved_stdout, STDOUT_FILENO); 
		dup2(*saved_stdin, STDIN_FILENO);
		close(*saved_stdout);
		close(*saved_stdin);
	}
}

static void	handle_expansions(t_shell *shell)
{
	t_dll	*curr;
	t_dll	*next;

	if (shell->ast->token)
		expansion(shell, &shell->ast->token, shell->ast->token);
	curr = shell->ast->arguments;
	while (curr)
	{
		next = curr->next;
		expansion(shell, &shell->ast->arguments, curr);
		curr = next;
	}
}

static void remove_redir_args(t_dll **args)
{
	t_dll *curr;
	
	curr = *args;
	while(curr)
	{
		if(curr->token_type == REDIRECTION)
			remove_token(args, curr);
		curr = curr->next;
	}
}

void execute_command(t_shell *shell)
{
    if (!shell->ast->token && !shell->ast->arguments)
        return;
    if (!redirections(shell, &shell->ast->token)
		&& !redirections(shell, &shell->ast->arguments))
    {
		handle_expansions(shell);
        if (shell->ast->arguments)
        {
            remove_redir_args(&shell->ast->arguments);
        }
        if (shell->ast->token && shell->ast->token->token_type == WORD)
        {
            if (is_builtin(shell))
			{
                execute_builtin(shell);
				set_last_cmd_env(shell);
			}
            else
                execute_external(shell);
        }
    }
    else
        shell->exit_code = 1;
}

void	execute_simple_command(t_shell *shell)
{
	int	saved_stdout;
	int	saved_stdin;

	if (!(shell->ast->token && (shell->ast->token->token_type == WORD ||
		shell->ast->token->token_type == REDIRECTION)))
		return ;
	save_restore_fds(&saved_stdout, &saved_stdin, 0);
	execute_command(shell);
	save_restore_fds(&saved_stdout, &saved_stdin, 1);
}
