/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:17:37 by iammar            #+#    #+#             */
/*   Updated: 2025/06/23 13:34:41 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

void	declare(t_shell *shell)
{
	t_env	*env;

	env = shell->env_list;
	if (!env)
		return ;
	while (env)
	{
		if (ft_strcmp(env->env_name, "_"))
			printf("declare -x %s=\"%s\"\n", env->env_name, env->env_value);
		env = env->next;
	}
}

int	var_check(char *val, t_shell *shell)
{
	int		i;
	char	*name;

	name = NULL;
	i = is_valid_identifier(val);
	if (!i)
		return (1);
	if (val[i] == '=')
	{
		name = ft_strduplen(shell, val, i);
		if (!val[i + 1])
			set_env_var(shell, &shell->env_list, name, NULL);
		else
		{
			i += 1;
			set_env_var(shell, &shell->env_list, name, &val[i]);
		}
	}
	return (0);
}

void	execute_builtin_export(t_shell *shell)
{
	t_dll	*curr;
	int		e_code;
	char	*tmp;

	e_code = 0;
	curr = shell->ast->arguments;
	if (!curr)
		return (declare(shell));
	while (curr)
	{
		if (var_check(curr->value, shell))
		{
			tmp = ft_strjoin(shell, "export: `", curr->value);
			ft_putstr_fd(ft_strjoin(shell, tmp, \
				"': not a valid identifier\n"), 2);
			e_code = 1;
		}
		curr = curr->next;
	}
	shell->exit_code = e_code;
}
