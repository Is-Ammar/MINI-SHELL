/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/04 09:17:36 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h> 

typedef struct s_env
{
	char				*env_name;
	char				*env_value;
	int 				exported;
	struct s_env		*next;
}						t_env;

typedef struct s_shell	t_shell;
typedef struct s_dll	t_dll;

typedef	struct s_ast
{
	t_dll 			*token;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//------------------------------Execution--------------------//
int						is_builtin(t_shell *shell);
void					execute_builtin(t_shell *shell);
void					execution(t_shell *shell);

//------------------------buitlin-----------------------------//

void					execute_builtin_cd(t_shell *shell);
void					execute_builtin_pwd(t_shell *shell);
void					execute_builtin_env(t_shell *shell);
void 					execute_builtin_exit(t_shell *shell);
void					execute_builtin_echo(t_shell *shell);
void					execute_builtin_export(t_shell *shell);
void 					execute_builtin_unset(t_shell *shell);

//-----------------------Utils-------------------------------//
int						ft_strcmp(char *s1, char *s2);
void					ft_putstr_fd(char const *s, int fd);
char					*ft_strdup(const char *str);
size_t					ft_strlcpy(char *dst, char *src, size_t len);
double					ft_atoi(char *str);
int						ft_isalpha(int c);
int						ft_isalnum(int c);
int						is_valid_identifier(const char *str);
char 					*get_current_dir_safe(t_shell *shell);

//---------------------Env-settings-------------------------//
void					free_env_list(t_env *env_list);
char					*get_env_var(t_env *env_list, char *name);
void					set_env_var(t_env **env_list, char *name, char *value);
t_env					*create_env_var(const char *name, const char *value);
void					add_env_var(t_env **env_list, const char *name,
							const char *value);
void 					unset_env_var(t_shell *shell, t_dll *token);

//-------------------Create-new-env--------------------------//
void					create_env(t_env **env, char **environ);

#endif