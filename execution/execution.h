/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/26 00:57:21 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_env
{
	char				*env_name;
	char				*env_value;
	struct s_env		*next;
}						t_env;

typedef struct s_dll	t_dll;
//------------------------------Execution--------------------//
int						is_builtin(t_dll **tokens);
void					execute_builtin(t_dll *tokens, t_env **env);
void					execute_input(t_dll **tokens, t_env **environ);

//------------------------buitlin-----------------------------//

void					execute_builtin_cd(t_dll *tokens, t_env **env);
void					execute_builtin_pwd(t_env *env);
void					execute_builtin_env(t_dll *tokens, t_env *env);
void 					execute_builtin_exit(t_dll *tokens);
void					execute_builtin_echo(t_dll *tokens);

//-----------------------Utils-------------------------------//
int						ft_strcmp(char *s1, char *s2);
void					ft_putstr_fd(char const *s, int fd);
char					*ft_strdup(const char *str);
size_t					ft_strlcpy(char *dst, char *src, size_t len);

//---------------------Env-settings-------------------------//
void					free_env_list(t_env *env_list);
char					*get_env_var(t_env *env_list, char *name);
void					set_env_var(t_env **env_list, char *name, char *value);
t_env					*create_env_var(const char *name, const char *value);
void					add_env_var(t_env **env_list, const char *name,
							const char *value);

//-------------------Create-new-env--------------------------//
void					create_env(t_env **env, char **environ);

#endif