/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smash.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/19 16:34:51 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMASH_H
# define SMASH_H

# include "parsing/parsing.h"
# include "execution/execution.h"

# define RESET   "\001\033[0m\002"
# define BG_BLACK   "\001\033[40m\002"

# define B_RED		"\001\033[1;31m\002"
# define B_GREEN	"\001\033[1;32m\002"
# define B_YELLOW	"\001\033[1;33m\002"
# define B_BLUE		"\001\033[1;34m\002"
# define B_PURPLE	"\001\033[1;35m\002"
# define B_CYAN		"\001\033[1;36m\002"
# define B_WHITE	"\001\033[1;37m\002"
# define B_GRAY		"\001\033[1;90m\002"

typedef struct s_dll	t_dll;
typedef struct s_env	t_env;
typedef struct s_ast	t_ast;
typedef struct s_gc		t_gc;

extern int				g_received;

typedef struct s_shell
{
	t_env	*env_list;
	t_dll	*tokens;
	t_ast	*ast;
	t_gc	*g_collect;
	int		exit_code;
	int		lines;
	int		interactive;
}	t_shell;

void printtt(t_ast *root);

#endif