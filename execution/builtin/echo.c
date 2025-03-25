/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/25 02:57:54 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	execute_builtin_echo(t_dll *tokens)
{
	t_dll   *curr;
	int		flag;

	if (!tokens->next || (tokens->next && tokens->next->token_type != WORD))
	{
		printf("\n");
		return ;
	}
	flag = 0;
	curr = tokens->next;
	if (!ft_strcmp(curr->value, "-n"))
	{
		flag = 1;
		curr = curr->next;
	}
	while (curr && curr->token_type == WORD)
	{
		printf("%s", curr->value);
		curr = curr->next;
		if (curr && curr->token_type == WORD)
			printf(" ");
	}
	if (!flag)
		printf("\n");
}
