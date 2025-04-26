/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/25 11:27:29 by habdella         ###   ########.fr       */
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

void	expandable_doc(char *delim, char *name, t_env *env, int e_code)
{
	char	*line;
	char	*temp;
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
			temp = line;
			line = expand_in_heredoc(line, env, e_code);
			free(temp);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
}

void	handle_herdoc(t_dll *nxt, char *name, t_env *env, int e_code)
{
	pid_t		pid;
	char		*temp;
	int			state;

	pid = fork();
	if (pid == 0)
	{
		if (nxt->quote_type == NONE)
			expandable_doc(nxt->value, name, env, e_code);
		else if (nxt->quote_type != NONE)
		{
			temp = nxt->value;
			nxt->value = remove_quotes(nxt->value);
			free(temp);
			non_expandable_doc(nxt->value, name);
		}
	}
	else
		waitpid(pid, &state, 0);
	return ;
}

void	heredoc(t_dll **tokens, t_env *env, int e_code)
{
	t_dll	*curr;
	char	*temp;
	char	*name;
	int		count;

	if (!tokens || !*tokens)
		return ;
	(1) && (curr = *tokens, count = 0, name = ft_strdup(""));
	while (curr && curr->next)
	{
		if (curr->token_type == OPERATOR)
			count++;
		if (curr->heredoc == TRUE)
		{
			temp = name;
			name = ft_strjoin(ft_strdup("/tmp/.heredoc_"), ft_itoa(count));
			free(temp);
			handle_herdoc(curr->next, name, env, e_code);
			replace_tokens(tokens, curr, curr->next, name);
		}
		curr = curr->next;
	}
	free(name);
	last_check_doc(tokens);
}
