/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:35:17 by iammar            #+#    #+#             */
/*   Updated: 2025/05/18 15:05:43 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	save_restore_fds(int *saved_stdout, int *saved_stdin, int restore)
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
void add_arg(t_dll **args, t_dll *curr)
{
	t_dll *current = *args;
	while(current)
	{
		current=current->next;
	}
	current = curr;
}
int	handle_expansions(t_shell *shell)
{
	t_dll	*curr;

	if (shell->ast->token)
	{
		if (expansion(shell, &shell->ast->token, &shell->ast->token))
			return (1);	
	}
	curr = shell->ast->arguments;
	while (curr)
	{
		// ft_printf("token :%s | token type: %d\n", curr->value, curr->expandable);
		if (shell->ast->token == curr)
			;
		else if (curr->heredoc)
		{
			if (curr->expandable)
				expand_heredoc(shell, curr->value);
		}
		else
		{
			if (expansion(shell, &shell->ast->arguments, &curr))
				return (1);
		}
		curr = curr->next;
	}
	return (0);
}

void execute_command(t_shell *shell)
{
    if (!shell->ast->token && !shell->ast->arguments)
	{
    	return;
	}
	// printf("token:%s\n", shell->ast->token->value);
	// printf("type:%d\n", shell->ast->token->token_type);
    if (handle_expansions(shell))
	{
		shell->exit_code = 1;
		return ;
	}
	// if (shell->ast->arguments)
	// {
	// 	remove_redir_args(&shell->ast->arguments);
	// }
	// t_dll	*curr = shell->ast->arguments;
	// while(curr)
	// {
	// 	printf("args: %s\n", curr->value);
	// 	printf("type: %d\n", curr->token_type);
	// 	curr = curr->next;
	// }
	// exit(0);
	
    if (shell->ast->token && shell->ast->token->token_type == WORD)
    {
        if (is_builtin(shell))
        {
            set_last_cmd_env(shell);
            execute_builtin(shell);
        }
        else
            execute_external(shell);
    }
}

void	execute_simple_command(t_shell *shell)
{

	if (!(shell->ast->token && (shell->ast->token->token_type == WORD)))
		return ;
	execute_command(shell);
}
