/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/20 15:33:04 by habdella         ###   ########.fr       */
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

void	check_overflow(char *str, int *overflow)
{
	int				i;
	int				sign;
	unsigned long	check;

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
		check = check * 10 + (str[i] - '0');
		if (sign == 1 && check > LONG_MAX)
			*overflow = 1;
		if (sign == -1 && check - 1 > LONG_MAX)
			*overflow = 1;
		i++;
	}
}

int	ft_atol(char *s)
{
	long	num;
	int		sign;

	num = 0;
	sign = 1;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign *= -1;
	while (*s >= '0' && *s <= '9')
	{
		num = (*s - '0') + (num * 10);
		s++;
	}
	return ((long)(num * sign));
}

static int	validate_and_convert_arg(char *arg, int *status)
{
	long	result;
	int		overflow;

	if (!arg || !is_valid(arg))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		*status = 2;
		return (0);
	}
	overflow = 0;
	result = ft_atol(arg);
	check_overflow(arg, &overflow);
	if (overflow)
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
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
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			shell->exit_code = 1;
			return ;
		}
	}
	clean_exit(shell, status);
}
