/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smash.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/21 14:02:30 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMASH_H
# define SMASH_H

#include "parsing/parsing.h"
#include "execution/execution.h"


#define GREEN   "\001\033[32m\002"
#define YELLOW  "\001\033[33m\002"
#define BLUE    "\001\033[34m\002"
#define MAGENTA "\001\033[35m\002"
#define WHITE   "\001\033[37m\002"
#define BLACK   "\001\033[30m\002"


#define BOLD    "\001\033[1m\002"
#define RESET   "\001\033[0m\002"
#define ITALIC  "\001\033[3m\002"
#define UNDERLINE "\001\033[4m\002"


#define BG_RED     "\001\033[41m\002"
#define BG_GREEN   "\001\033[42m\002"
#define BG_YELLOW  "\001\033[43m\002"
#define BG_BLUE    "\001\033[44m\002"
#define BG_MAGENTA "\001\033[45m\002"
#define BG_CYAN    "\001\033[46m\002"
#define BG_WHITE   "\001\033[47m\002"
#define BG_BLACK   "\001\033[40m\002"

typedef struct s_shell
{
    t_env   *env_list;
    t_dll   *tokens;
    t_ast   *ast;
    int     subshell;
    int     exit_code;
}   t_shell;

typedef struct s_dll	t_dll;
typedef	struct s_env	t_env;
typedef struct s_ast    t_ast;

#endif