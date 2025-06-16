/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hered_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/15 18:23:15 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

char	*ft_strnstr(const char *big, const char *little, int len)
{
	int	i;
	int	j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (i < len && big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && (i + j) < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)(big + i));
			j++;
		}
		i++;
	}
	return (NULL);
}

int	get_name_number(t_shell *shell, int count)
{
	int		fd;
	long	num;
	char	*name_fd;
	char	*name_tty;

	fd = open("/proc/self/stat", O_RDONLY);
	name_fd = ft_malloc(shell, 8, 0);
	name_fd[7] = '\0';
	read(fd, name_fd, 7);
	close(fd);
	name_tty = ttyname(STDIN_FILENO);
	num = ft_atoi(ft_strjoin(shell, name_fd, &name_tty[9]));
	return (num + count);
}

static char	*doc_dollar(t_shell *shell, char *value, int *i)
{
	int		len;
	int		start;
	int		e_code;
	char	*p;

	p = NULL;
	e_code = shell->exit_code;
	start = *i + 1;
	len = start;
	if (value[len] == '?')
		return (*i = len + 1, ft_itoa(shell, e_code, 0));
	if (!ft_isalnum(value[len]) && value[len] != '_')
		return (*i = len, ft_strdup(shell, "$"));
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
		len++;
	*i = len;
	p = ft_strduplen(shell, &value[start], len - start);
	return (ft_strdup_expand(shell, p));
}

char	*expand_in_heredoc(t_shell *shell, char *value)
{
	char	*new_val;
	int		i;
	int		j;

	i = 0;
	new_val = ft_strdup(shell, "");
	while (value[i])
	{
		j = i;
		while (value[i] && value[i] != '$')
			i++;
		if (j != i)
			new_val = ft_strjoin(shell, new_val, ft_strduplen(shell, &value[j] \
			, i - j));
		else if (value[i] == '$')
			new_val = ft_strjoin(shell, new_val, doc_dollar(shell, value, &i));
	}
	return (new_val);
}
