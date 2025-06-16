/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/16 15:05:05 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	copy_new_content(t_shell *shell, char *new_name, char *old_name)
{
	char	*line;
	int		fd0;
	int		fd1;

	fd0 = open(new_name, O_RDONLY, 0600);
	fd1 = open(old_name, O_WRONLY | O_TRUNC, 0600);
	line = get_next_line(shell, fd0);
	while (line != NULL)
	{
		write(fd1, line, ft_strlen(line));
		line = get_next_line(shell, fd0);
	}
	close(fd0);
	close(fd1);
	unlink(new_name);
}

void	expand_heredoc(t_shell *shell, char *old_name)
{
	char	*line;
	char	*name;
	int		fd0;
	int		fd1;

	name = ft_strjoin(shell, old_name, ft_itoa(shell, shell->lines, 0));
	fd0 = open(old_name, O_RDONLY, 0600);
	fd1 = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	line = get_next_line(shell, fd0);
	while (line != NULL)
	{
		if (ft_strchr(line, '$'))
			line = expand_in_heredoc(shell, line);
		write(fd1, line, ft_strlen(line));
		line = get_next_line(shell, fd0);
	}
	close(fd0);
	close(fd1);
	copy_new_content(shell, name, old_name);
}

void	open_heredoc(t_shell *shell, char *delim, char *name)
{
	char	*line;
	int		fd;

	line = NULL;
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0600);
	while (1)
	{
		line = readline(B_PURPLE"heredoc> "RESET);
		if (!line)
		{
			ft_printf("minishell: warning: here-document at line %d" \
			, shell->lines);
			ft_printf(" delimited by end-of-file (wanted `%s')\n", delim);
			break ;
		}
		else
			shell->lines++;
		if (!ft_strcmp(line, delim))
			break ;
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

	nxt->value = expand_delim(shell, nxt->value);
	if (nxt->quote_type == NONE)
		nxt->expandoc = TRUE;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		open_heredoc(shell, nxt->value, name);
		clean_exit(shell, 0);
	}
	else
		waitpid(pid, &state, 2);
	if (WIFEXITED(state) && WEXITSTATUS(state) == 130)
	{
		shell->exit_code = 130;
		g_received = SIGINT;
		write(STDERR_FILENO, "\n", 1);
		return ;
	}
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
		count++;
		if (curr->heredoc == TRUE)
		{
			name = ft_strjoin(shell, ft_strdup(shell, "/tmp/.heredoc_") \
			, ft_itoa(shell, get_name_number(shell, count), 0));
			handle_herdoc(shell, curr->next, name);
			if (g_received == SIGINT)
				return ;
			curr->expandoc = curr->next->expandoc;
			remove_token(tokens, curr->next);
			add_mid_token(shell, tokens, curr, name);
			remove_token(tokens, curr);
		}
		curr = curr->next;
	}
}
