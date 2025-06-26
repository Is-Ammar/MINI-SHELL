/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/26 08:26:50 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	too_much_new_lines(t_dll **curr, int *flag)
{
	int	i;

	while ((*curr) && (*curr)->value[0] == '-' && (*curr)->value[1] == 'n')
	{
		i = 2;
		while ((*curr)->value[i] == 'n')
			i++;
		if ((*curr)->value[i] == '\0')
		{
			*flag = 1;
			(*curr) = (*curr)->next;
		}
		else
			break ;
	}
	return ;
}

void	execute_builtin_echo(t_shell *shell)
{
	t_dll	*curr;
	int		flag;

	if (!shell->ast || !shell->ast->token || !shell->ast->arguments)
	{
		writing(1, "\n", 1);
		return ;
	}
	flag = 0;
	curr = shell->ast->arguments;
	too_much_new_lines(&curr, &flag);
	while (curr)
	{
		ft_putstr_fd(curr->value, 1);
		if (curr->next)
			writing(1, " ", 1);
		curr = curr->next;
	}
	if (!flag)
		writing(1, "\n", 1);
	shell->exit_code = 0;
}
