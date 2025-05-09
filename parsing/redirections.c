/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/09 15:45:19 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	out_fd(t_shell *shell, t_dll **tokens, t_dll *token, int O_FLAG)
{
	int	out_fd;

	out_fd = 1;
	if (token->expandable || token->wildcard)
	{
		if (expansion(shell, tokens, token))
			return (1);
	}
	if (access(token->value, F_OK) == 0)
	{
		if (access(token->value, W_OK) == -1)
		{
			ft_printf(B_RED"minishell: %s: permission denied\n"RESET, token->value);
			return (1);
		}
	}
	out_fd = open(token->value, O_CREAT | O_WRONLY | O_FLAG, 0644);
	dup2(out_fd, 1);
	close(out_fd);
	return (0);
}

int	in_fd(t_shell *shell, t_dll **tokens, t_dll *token)
{
	int	in_fd;

	in_fd = 0;
	if (token->expandable || token->wildcard)
	{
		if (expansion(shell, tokens, token))
			return (1);
		if (token->next)
			token = token->next;
	}
	if (access(token->value, F_OK) == -1)
	{
		return (ft_error(token->value, EDIRFILE), 1);
	}
	if (access(token->value, R_OK) == -1)
	{
		ft_printf(B_RED"minishell: %s: permission denied\n"RESET, token->value);
		return (1);
	}
	in_fd = open(token->value, O_RDONLY);
	dup2(in_fd, 0);
	close(in_fd);
	return (0);
}

int	redirections(t_shell *shell, t_dll **tokens)
{
	t_dll	*curr;

	if (!tokens || !*tokens)
		return (0);
	curr = *tokens;
	while (curr && curr->token_type != OPERATOR && curr->token_type != PIPE)
	{
		if (curr->redir_type == READ)
		{
			if (in_fd(shell, tokens, curr))
				return (1);
		}
		else if (curr->redir_type == WRITE)
		{
			if (out_fd(shell, tokens, curr, O_TRUNC))
				return (1);
		}
		else if (curr->redir_type == APPEND)
		{
			if (out_fd(shell, tokens, curr, O_APPEND))
				return (1);
		}
		curr = curr->next;
	}
	return (0);
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
				remove_token(tokens, curr);
		}
		curr = nxt;
	}
	identify_redirections(tokens);
}
