/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/09 08:48:27 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int expand_execute(t_shell *shell, t_dll **tokens, t_dll *curr)
{
    char	*tmp;

    tmp = curr->value;
    curr->value = expand_env_vars(shell, curr->value);
	curr->expandable = FALSE;
	if (curr->token_type == REDIRECTION && ft_strchr(tmp, '$')
        && curr->value[0] == '\0')
	{
		ft_printf(B_WHITE"minishell: %s: ambiguous redirect\n"RESET, tmp);
		curr->ambiguous = TRUE;
		remove_token(tokens, curr);
		return (1);
	}
	if (curr->value[0] == '\0')
		remove_token(tokens, curr);
    return (0);
}