/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/21 23:07:54 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	execute_builtin_pwd(t_shell *shell)
{
	char	*cwd;
	char	*tmp_cwd;

	cwd = getcwd(NULL, 0);
	tmp_cwd = ft_strdup(shell, cwd);
	free(cwd);
	if (!tmp_cwd)
	{
		tmp_cwd = ft_strdup(shell, shell->pwd);
	}
	printf("%s\n", tmp_cwd);
	shell->exit_code = 0;
}
