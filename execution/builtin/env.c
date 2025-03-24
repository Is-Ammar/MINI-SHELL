/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/24 19:53:49 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void execute_builtin_env(t_dll *tokens, t_env *env)
{
    t_env *tmp;

    if (!tokens)
        return;
    if (!env)
        return;

    if (tokens->next)
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
