/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/22 11:46:42 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	execute_builtin_pwd(t_shell *shell)
{
	if (shell->pwd)
	{
		printf("%s\n", shell->pwd);
		shell->exit_code = 0;
		return ;
	}
	printf("pwd: error retrieving current directory\n");
	shell->exit_code = 1;
}
