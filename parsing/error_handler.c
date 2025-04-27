/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/27 13:51:29 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "stdarg.h"

int	putlchar(char c)
{
	write(2, &c, 1);
	return (1);
}

int	putlstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (i += putlstr("(null)"));
	while (s[i])
	{
		write (2, &s[i], 1);
		i++;
	}
	return (i);
}

int	check_format(va_list list, const char *format, int i)
{
	if (format[i] == 'c')
		return (putlchar(va_arg(list, int)));
	else if (format[i] == 's')
		return (putlstr(va_arg(list, char *)));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	list;
	int		len;
	int		i;

	i = 0;
	len = 0;
	if (!format)
		return (-1);
	va_start(list, format);
	while (format[i])
	{
		if (format[i] == '%' && ft_strchr("cs\%", format[i + 1]))
		{
			len += check_format(list, format, i + 1);
			i += 2;
		}
		else
		{
			len += putlchar(format[i]);
			i++;
		}
	}
	va_end(list);
	return (len);
}

int	ft_error(char *val, t_error_type error)
{
	if (error == EQUOTES)
	{
		ft_printf(RED"minishell: unexpected EOF while looking for matching ");
		ft_printf("`%c'\n"RESET, *val);
	}
	else if (error == EBRACKET)
	{
		ft_printf(RED"minishell: syntax error near unexpected token ");
		ft_printf("`)'\n"RESET);
	}
	else if (error == ESYNTAX)
	{
		ft_printf(RED"minishell: syntax error near unexpected token ");
		ft_printf("`%s'\n"RESET, val);
	}
	else if (error == ECOMMAND)
	{
		if (ft_strchr(val, '/'))
			ft_printf(B_WHITE"minishell: %s: No such file or directory\n"RESET \
			, val);
		else
			ft_printf(B_WHITE"%s: command not found\n"RESET, val);
		return (127);
	}
	return (2);
}
