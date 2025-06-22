/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 08:13:13 by habdella          #+#    #+#             */
/*   Updated: 2025/06/22 11:37:16 by habdella         ###   ########.fr       */
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
	tmp = ft_strjoin(shell, line1, \
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

	username = getenv("USER");
	if (!username)
		username = "user";
	cwd = shell->pwd;
	if (!cwd)
		cwd = ft_strdup(shell, "?");
	return (join_prompt(shell, cwd, username));
}

void	init_shell_pwd(t_shell *shell)
{
	char	*cwd;

	shell->pwd = NULL;
	cwd = getcwd(NULL, 0);
	if (cwd)
		shell->pwd = env_strdup(shell, cwd);
	else
	{
		ft_putstr_fd("init_shell_pwd: error retrieving current directory: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
	}
	free(cwd);
}
