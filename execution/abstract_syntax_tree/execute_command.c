/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:35:17 by iammar            #+#    #+#             */
/*   Updated: 2025/06/16 13:01:11 by habdella         ###   ########.fr       */
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

int	handle_expansions(t_shell *shell)
{
	t_dll	*curr;
	t_dll	*nxt;

	curr = shell->ast->token;
	while (curr)
	{
		nxt = curr->next;
		if (expansion(shell, &shell->ast->token, &curr))
			return (1);
		curr = nxt;
	}
	curr = shell->ast->arguments;
	while (curr)
	{
		nxt = curr->next;
		if (shell->ast->token != curr)
		{
			if (expansion(shell, &shell->ast->arguments, &curr))
				return (1);
		}
		curr = nxt;
	}
	return (0);
}

t_dll	*add(t_shell *shell, t_dll **head, char *val)
{
	t_dll	*token;
	t_dll	*curr;

	token = create_token_list(shell);
	token->value = ft_strdup(shell, val);
	token->token_type = WORD;
	if (ft_strchr(val, '*'))
		token->wildcard = TRUE;
	if (!*head)
	{
		*head = token;
		return (token);
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = token;
	token->prev = curr;
	token->next = NULL;
	return (token);
}

void	execute_command(t_shell *shell)
{
	if (shell->ast->token && shell->ast->token->token_type == WORD
		&& shell->ast->token->fake_cmd == FALSE)
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
	t_dll	*curr;
	t_dll	*tmp;

	if (!(shell->ast->token && (shell->ast->token->token_type == WORD)))
		return ;
	if (!shell->ast->token && !shell->ast->arguments)
		return ;
	if (handle_expansions(shell))
	{
		shell->exit_code = 1;
		return ;
	}
	curr = shell->ast->token;
	if (shell->ast->token && shell->ast->token->next)
	{
		while (curr && curr->next)
		{
			curr = curr->next;
			tmp = add(shell, &shell->ast->arguments, curr->value);
			remove_token(&shell->ast->token, curr);
			expansion(shell, &shell->ast->arguments, &tmp);
		}
	}
	execute_command(shell);
}
