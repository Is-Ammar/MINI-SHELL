/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/12 09:22:36 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	exec_error(char *val, t_error_type error)
{
	if (error == ECOMMAND)
	{
		ft_printf(B_WHITE"minishell: %s: command not found\n"RESET, val);
		return (127);
	}
	else if (error == EDIRFILE)
	{
		ft_printf(B_WHITE"minishell: %s: No such file or directory\n"RESET, val);
		return (127);
	}
	else if (error == EPERMISS)
	{
		ft_printf(B_RED"minishell: %s: permission denied\n"RESET, val);
		return (1);
	}
	return (0);
}

int	parse_error(char *val, t_error_type error)
{
	if (error == EQUOTES)
	{
		ft_printf(B_RED"minishell: unexpected EOF while looking for matching");
		ft_printf(" `%c'\n"RESET, *val);
	}
	else if (error == EBRACKET)
	{
		ft_printf(B_RED"minishell: syntax error near unexpected token ");
		ft_printf("`)'\n"RESET);
	}
	else if (error == ESYNTAX)
	{
		ft_printf(B_RED"minishell: syntax error near unexpected token ");
		ft_printf("`%s'\n"RESET, val);
	}
	else if (error == EAMBIGUO)
	{
		ft_printf(B_GRAY"minishell: %s: ambiguous redirect\n"RESET, val);
		return (1);
	}
	return (2);
}
