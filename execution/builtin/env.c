/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 16:09:17 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	ft_error(t_shell *shell, char *s1, char *s2, char *s3)
{
	char	*tmp;

	tmp = ft_strjoin(shell, s1, s2);
	ft_putstr_fd(ft_strjoin(shell, tmp, s3), 2);
}

void	check_argument(t_shell *shell)
{
	char		*arg;
	struct stat	file_stat;

	if (shell->ast->token && shell->ast->arguments)
	{
		arg = shell->ast->arguments->value;
		if (stat(arg, &file_stat) == 0)
		{
			if (opendir(arg) || (access(arg, X_OK) == -1))
			{
				ft_error(shell, "env: ", arg, ": Permission denied\n");
				shell->exit_code = 126;
				return ;
			}
		}
		else
		{
			ft_error(shell, "env: ", arg, ": No such file or directory\n");
			shell->exit_code = 127;
			return ;
		}
		shell->exit_code = 1;
		return ;
	}
}

void	execute_builtin_env(t_shell *shell)
{
	t_env	*tmp;

	if (!shell->tokens || !shell->env_list)
		return ;
	tmp = shell->env_list;
	while (tmp)
	{
		printf("%s=", tmp->env_name);
		if (tmp->env_value)
			printf("%s", tmp->env_value);
		printf("\n");
		tmp = tmp->next;
	}
	shell->exit_code = 0;
}
