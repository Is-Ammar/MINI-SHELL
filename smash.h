/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smash.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 21:39:36 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMASH_H
# define SMASH_H

#include "parsing/parsing.h"
#include "execution/execution.h"

typedef struct s_shell
{
    t_env   *env_list;
    t_dll   *tokens;
    int     exit_code;
}   t_shell;




#endif
