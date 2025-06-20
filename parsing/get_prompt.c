/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 08:13:13 by habdella          #+#    #+#             */
/*   Updated: 2025/06/20 08:15:34 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*join_prompt(t_shell *shell, char *cwd, char *username)
{
	char	*line1;
	char	*line2;
	char	*tmp;

	tmp = ft_strjoin(shell, B_GREEN, username);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1,
			RESET B_WHITE "@" B_GRAY "minishell" B_WHITE " ❯ ");
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, B_BLUE "┃" B_CYAN BG_BLACK);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, cwd);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, RESET B_BLUE "┃\n" RESET);
	line2 = ft_strdup(shell, B_YELLOW "\001➔\002 \x7f" RESET);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, line2);
	return (tmp);
}

char	*get_prompt(t_shell *shell)
{
	char	*cwd;
	char	*username;
	char	*tmp;

	username = getenv("USER");
	if (!username)
		username = "user";
	cwd = getcwd(NULL, 0);
	tmp = ft_strdup(shell, cwd);
	if (cwd)
		free(cwd);
	if (!tmp)
		tmp = ft_strdup(shell, "~");
	return (join_prompt(shell, tmp, username));
}
