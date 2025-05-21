/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/19 08:05:05 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

#define BUFFER_SIZE 1337

char	*ft_strdupnl(t_shell *shell, char *contain)
{
	char	*line;
	int		i;
	int		j;

	if (!contain)
		return (NULL);
	i = 0;
	while (contain[i] && contain[i] != '\n')
		i++;
	if (contain[i] != '\0')
		i++;
	line = ft_malloc(shell, i + 1);
	j = 0;
	while (j < i)
	{
		line[j] = contain[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

char	*update(t_shell *shell, char *contain)
{
	char	*p;
	int		i;

	i = 0;
	if (!contain)
		return (NULL);
	while (contain[i] && contain[i] != '\n')
		i++;
	if (contain[i])
		i++;
	p = ft_strdup(shell, contain + i);
	return (p);
}

char	*get_container(t_shell *shell, char *contain, char *buff, int fd)
{
	ssize_t	reader;

	reader = read(fd, buff, BUFFER_SIZE);
	if (reader < 0 || !buff)
		return (NULL);
	while (reader > 0)
	{
		buff[reader] = '\0';
		if (!contain)
			contain = ft_strdup(shell, buff);
		else if (buff)
		{
			contain = ft_strjoin(shell, contain, buff);
		}
		if (ft_strchr(contain, '\n') || reader < BUFFER_SIZE)
			return (contain);
		reader = read(fd, buff, BUFFER_SIZE);
	}
	if (contain)
		return (contain);
	return (NULL);
}

char	*get_next_line(t_shell *shell, int fd)
{
	static char	*contain = NULL;
	char		*buff;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	buff = ft_malloc(shell, BUFFER_SIZE + 1);
	contain = get_container(shell, contain, buff, fd);
	if (!contain || !ft_strlen(contain))
		return (NULL);
	line = ft_strdupnl(shell, contain);
	contain = update(shell, contain);
	return (line);
}

char	*my_readline(t_shell *shell)
{
    char *buff;
	ssize_t bytes;
    char    c[2];

    buff = ft_malloc(shell, 1);
	bytes = read(STDIN_FILENO, &c, 1);
    while (bytes > 0)
    {
        c[1] = '\0';
        if (c[0] == '\n')
            break ;
        buff = ft_strjoin(shell, buff, c);
		bytes = read(STDIN_FILENO, &c, 1);
    }
	if (bytes <= 0)
		return (NULL);
    return (buff);
}