/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/17 12:09:44 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	out_fd(t_shell *shell, t_dll **tokens, t_dll *token, int O_FLAG)
{
	int	out_fd;

	if (expansion(shell, tokens, &token))
		return (1);
	out_fd = open(token->value, O_CREAT | O_WRONLY | O_FLAG, 0644);
	if (out_fd == -1)
	{
		if (errno == ENOENT)
			return (exec_error(shell, token->value, EDIRFILE), 1);
		if (errno == EACCES)
			return (exec_error(shell, token->value, EPERMISS), 1);
		if (errno == EISDIR)
			return (exec_error(shell, token->value, EISDIR), 1);
		if (errno == ENOTDIR)
			return (exec_error(shell, token->value, ENOTDIR), 1);
		else
			return (perror("minishell: "), 1);
	}
	dup2(out_fd, 1);
	close(out_fd);
	return (0);
}

int	in_fd(t_shell *shell, t_dll **tokens, t_dll *token)
{
	int	in_fd;

	if (token && token->expandoc)
		expand_heredoc(shell, token->value);
	if (expansion(shell, tokens, &token))
		return (1);
	in_fd = open(token->value, O_RDONLY);
	if (in_fd == -1)
	{
		if (errno == ENOENT)
			return (exec_error(shell, token->value, EDIRFILE), 1);
		if (errno == EACCES)
			return (exec_error(shell, token->value, EPERMISS), 1);
		if (errno == EISDIR)
			return (exec_error(shell, token->value, EISDIR), 1);
		if (errno == ENOTDIR)
			return (exec_error(shell, token->value, ENOTDIR), 1);
		else
			return (perror("minishell: "), 1);
	}
	dup2(in_fd, 0);
	close(in_fd);
	if (token->heredoc)
		unlink(token->value);
	return (0);
}

void	identify_redirections(t_dll **tokens)
{
	t_dll	*curr;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr)
	{
		if (ft_strnstr(curr->value, "/tmp/.heredoc_", 14))
		{
			curr->token_type = REDIRECTION;
			curr->heredoc = TRUE;
			curr->redir_type = READ;
		}
		curr = curr->next;
	}
}

int	handle_redirect(char *value, t_dll *nxt)
{
	if (!ft_strcmp(value, ">>"))
	{
		nxt->redir_type = APPEND;
		return (1);
	}
	if (!ft_strcmp(value, "<"))
	{
		nxt->redir_type = READ;
		return (1);
	}	
	if (!ft_strcmp(value, ">"))
	{
		nxt->redir_type = WRITE;
		return (1);
	}
	return (0);
}

void	redirect(t_dll **tokens)
{
	t_dll	*curr;
	t_dll	*nxt;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		nxt = curr->next;
		if (curr->direction == LEFT || curr->direction == RIGHT)
		{
			if (handle_redirect(curr->value, nxt))
			{
				nxt->token_type = REDIRECTION;
				remove_token(tokens, curr);
			}
		}
		curr = nxt;
	}
	identify_redirections(tokens);
}
