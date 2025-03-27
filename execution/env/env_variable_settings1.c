/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 21:11:14 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	create_env(t_env **env, char **environ)
{
	int		i;
	char	*equals_sign;
	char	*name;
	char	*value;
	int		name_len;

	i = 0;
	*env = NULL;
	while (environ[i])
	{
		equals_sign = ft_strchr((const char *)environ[i], '=');
		if (equals_sign)
		{
			name_len = equals_sign - environ[i];
			name = malloc(name_len + 1);
			if (!name)
				continue ;
			ft_strlcpy(name, environ[i], name_len + 1);
			name[name_len] = '\0';
			value = ft_strdup(equals_sign + 1);
			if (!value)
			{
				free(name);
				continue ;
			}
			add_env_var(env, name, value);
			free(name);
			free(value);
		}
		i++;
	}
}

void unset_env_var(t_shell *shell, t_dll *token)
{
    t_env *env;
    t_env *prev;

    env = shell->env_list;
    prev = NULL;

    while (env)
    {
        if (ft_strcmp(env->env_name, token->value) == 0)
        {
            if (prev)
                prev->next = env->next;
            else
                shell->env_list = env->next;
            
            free(env->env_name);
            free(env->env_value);
            free(env);
            return;
        }
        prev = env;
        env = env->next;
    }
}

// int mark_env_var_exported(t_env **env, char *name)
// {
//     t_env *current;
    
//     if (!env || !name || !*name)
//         return (1);

//     current = *env;
//     while (current)
//     {
//         if (ft_strcmp(current->env_name, name) == 0)
//         {
            
//             current->exported = 1;
//             return (0);
//         }
//         current = current->next;
//     }
//     /////////////////////
	
// }
