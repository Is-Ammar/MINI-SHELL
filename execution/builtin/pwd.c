/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/22 01:28:14 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	execute_builtin_pwd(t_shell *shell)
{
	char	*cwd;
	
	if (shell->pwd)
	{
		printf("%s\n", shell->pwd);
		shell->exit_code = 0;
		return;
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		shell->exit_code = 0;
		return;
	}
	printf("pwd: error retrieving current directory\n");
	shell->exit_code = 1;
}
