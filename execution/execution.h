/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/27 18:00:47 by iammar           ###   ########.fr       */
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
	int 				exported;
	struct s_env		*next;
}						t_env;

typedef struct s_shell	t_shell;
//------------------------------Execution--------------------//
int						is_builtin(t_shell *shell);
void					execute_builtin(t_shell *shell);
void					execution(t_shell *shell);

//------------------------buitlin-----------------------------//

void					execute_builtin_cd(t_shell *shell);
void					execute_builtin_pwd(t_env *env);
void					execute_builtin_env(t_shell *shell);
void 					execute_builtin_exit(t_shell *shell);
void					execute_builtin_echo(t_shell *shell);
void						execute_builtin_export(t_shell *shell);

//-----------------------Utils-------------------------------//
int						ft_strcmp(char *s1, char *s2);
void					ft_putstr_fd(char const *s, int fd);
char					*ft_strdup(const char *str);
size_t					ft_strlcpy(char *dst, char *src, size_t len);
double					ft_atoi(char *str);
int						ft_isalpha(int c);
int						ft_isalnum(int c);

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