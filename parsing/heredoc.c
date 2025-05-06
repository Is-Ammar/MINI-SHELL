/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/06 15:52:10 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	non_expandable_doc(char *delim, char *name)
{
	char	*line;
	int		fd;

	line = NULL;
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
		return (perror("minishell: "));
	while (1)
	{
		line = readline(PURPLE"heredoc> "RESET);
		if (!ft_strcmp(line, delim))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
}

void	expandable_doc(t_shell *shell, char *delim, char *name)
{
	char	*line;
	int		fd;

	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
		return (perror("minishell: "));
	line = NULL;
	while (1)
	{
		line = readline(PURPLE"heredoc> "RESET);
		if (!ft_strcmp(line, delim))
			break ;
		if (ft_strchr(line, '$'))
		{
			line = expand_in_heredoc(shell, line);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
}

void	handle_herdoc(t_shell *shell, t_dll *nxt, char *name)
{
	pid_t		pid;
	int			state;

	pid = fork();
	if (pid == 0)
	{
		if (nxt->quote_type == NONE)
			expandable_doc(shell, nxt->value, name);
		else if (nxt->quote_type != NONE)
		{
			nxt->value = remove_quotes(shell, nxt->value);
			non_expandable_doc(nxt->value, name);
		}
	}
	else
		waitpid(pid, &state, 0);
	return ;
}

void	heredoc(t_shell *shell, t_dll **tokens)
{
	t_dll	*curr;
	char	*name;
	int		count;

	if (!tokens || !*tokens)
		return ;
	(1) && (curr = *tokens, count = 0, name = ft_strdup(shell, ""));
	while (curr && curr->next)
	{
		if (curr->token_type == OPERATOR)
			count++;
		if (curr->heredoc == TRUE)
		{
			name = ft_strjoin(shell, ft_strdup(shell, "/tmp/.heredoc_") \
			, ft_itoa(shell, count));
			handle_herdoc(shell, curr->next, name);
			remove_token(tokens, curr->next);
			add_mid_token(shell, tokens, curr, name);
			remove_token(tokens, curr);
		}
		curr = curr->next;
	}
	last_check_doc(tokens);
}
