/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/19 13:56:17 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

int	is_valid(char *arg)
{
	int	i;

	i = 0;
	if (*arg == '\0')
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i] != '\0')
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	execute_builtin_exit(t_shell *shell)
{
	int		status;
	long	long_status;
	char	*arg;
	int		valid;
	t_dll	*arg_token;

	if (!shell->ast->token->fork)
		printf("exit\n");
	status = shell->exit_code;
	if (shell->ast && shell->ast->token && shell->ast->arguments)
	{
		arg_token = shell->ast->arguments;
		arg = arg_token->value;
		valid = is_valid(arg);
		if (!valid)
		{
			ft_printf("minishell: exit: numeric argument required\n");
			clean_exit(shell, 2);
		}
		long_status = ft_atoi(arg);
		if (arg_token->next && arg_token->next->value)
		{
			ft_printf("minishell: exit: too many arguments\n");
			shell->exit_code = 1;
			return ;
		}
		else
		{
			status = (int)long_status;
		}
	}
	clean_exit(shell, status);
}
