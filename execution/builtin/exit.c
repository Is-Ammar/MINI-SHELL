/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 00:20:01 by iammar           ###   ########.fr       */
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

int	check_overflow(char *str, long result)
{
	int		i;
	int		sign;
	long	check;

	i = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	check = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (check > (LONG_MAX - (str[i] - '0')) / 10)
			return (1);
		check = check * 10 + (str[i] - '0');
		i++;
	}
	if (sign == -1 && check > LONG_MAX)
		return (1);
	return (result != check * sign);
}

static int	validate_and_convert_arg(char *arg, int *status)
{
	long	result;

	if (!arg || !is_valid(arg))
	{
		ft_printf("minishell: exit: numeric argument required\n");
		*status = 2;
		return (0);
	}
	result = ft_atoi(arg);
	if (check_overflow(arg, result))
	{
		ft_printf("minishell: exit: numeric argument required\n");
		*status = 2;
		return (0);
	}
	*status = (unsigned char)result;
	return (1);
}

void	execute_builtin_exit(t_shell *shell)
{
	int		status;
	t_dll	*arg_token;

	if (!shell->ast->token->fork)
		printf("exit\n");
	status = shell->exit_code;
	if (shell->ast && shell->ast->token && shell->ast->arguments)
	{
		arg_token = shell->ast->arguments;
		if (!validate_and_convert_arg(arg_token->value, &status))
		{
			clean_exit(shell, status);
			return ;
		}
		if (arg_token->next && arg_token->next->value)
		{
			ft_printf("minishell: exit: too many arguments\n");
			shell->exit_code = 1;
			return ;
		}
	}
	clean_exit(shell, status);
}
