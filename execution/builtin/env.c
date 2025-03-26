/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/26 01:42:55 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void execute_builtin_env(t_dll *tokens, t_env *env)
{
	t_env *tmp;

	if (!tokens || !env)
		return;
	if (tokens->next && tokens->next->token_type == WORD)
	{
		printf("env: '%s': No such file or directory\n", tokens->next->value);
		return;
	}
	tmp = env;
	while (tmp)
	{
		printf("%s=", tmp->env_name);
		if (tmp->env_value)
			printf("%s", tmp->env_value);
		printf("\n");
		tmp = tmp->next;
	}
}
