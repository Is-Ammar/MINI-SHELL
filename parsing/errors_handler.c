/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/22 10:01:08 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	exec_error(t_shell *shell, char *val, t_error_type error)
{
	if (error == ECOMMAND)
	{
		if (shell->interactive)
			ft_printf(B_WHITE"minishell: %s: command not found\n"RESET, val);
		else
			ft_printf(B_WHITE"minishell: line %d: %s: command not found\n"RESET \
			, shell->lines, val);
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
	else if (error == EISDIR)
		ft_printf(B_BLUE"minishell: %s: Is a directory\n"RESET, val);
	else if (error == ENOTDIR)
		ft_printf(B_BLUE"minishell: %s: Not a directory\n"RESET, val);
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

void	writing(int fd, char *s, size_t n)
{
	int	ret_val;

	ret_val = write(fd, s, n);
	if (ret_val == -1)
	{
		perror("minishell: write error: Bad file descriptor");
	}
}
