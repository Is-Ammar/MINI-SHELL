/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/06/18 17:10:00 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_env
{
	char				*env_name;
	char				*env_value;
	struct s_env		*next;
}						t_env;

typedef struct s_shell	t_shell;
typedef struct s_dll	t_dll;

typedef struct s_ast
{
	t_dll				*token;
	t_dll				*arguments;
	struct s_ast		*left;
	struct s_ast		*right;
	int					forked;
}						t_ast;

//------------------------------Execution--------------------//
int						is_builtin(t_shell *shell);
void					execute_builtin(t_shell *shell);
void					execution(t_shell *shell);
void					execute_ast(t_shell *shell);
void					execute_external(t_shell *shell);
int						execute_pipe(t_shell *shell);
void					execute_subshell(t_shell *shell);

//------------------------buitlin-----------------------------//

void					execute_builtin_cd(t_shell *shell);
void					execute_builtin_pwd(t_shell *shell);
void					execute_builtin_env(t_shell *shell);
void					execute_builtin_exit(t_shell *shell);
void					execute_builtin_echo(t_shell *shell);
void					execute_builtin_export(t_shell *shell);
void					execute_builtin_unset(t_shell *shell);

//-----------------------Utils-------------------------------//
int						ft_strcmp(char *s1, char *s2);
void					ft_putstr_fd(char const *s, int fd);
char					*ft_strdup(t_shell *shell, const char *str);
size_t					ft_strlcpy(char *dst, char *src, size_t len);
double					ft_atoi(char *str);
int						ft_isalpha(int c);
int						ft_isalnum(int c);
int						is_valid_identifier(const char *str);
char					*ft_strcat(char *dest, const char *src);
char					**ft_split(t_shell *shell, char const *s, char c);
void					ft_putchar_fd(char c, int fd);
//---------------------Env-settings-------------------------//
char					*get_env_var(t_shell *shell, t_env *env_list,
							char *name);
void					set_env_var(t_shell *shell, t_env **env_list,
							char *name, char *value);
void					add_env_var(t_shell *shell, t_env **env_list,
							char *name, char *value);
void					unset_env_var(t_shell *shell, t_dll *token);
char					**convert_env_to_array(t_shell *shell, t_env *env_list);
void					set_last_cmd_env(t_shell *shell);

//-------------------Create-new-env--------------------------//
void					create_env(t_shell *shell, t_env **env, char **environ);
//-------------------abstract_tree---------------------------//
t_ast					*abstract_segment_tree(t_shell *shell);
t_ast					*parse_redirections(t_dll **tokens, t_shell *shell);
t_ast					*parse_logical_operators(t_dll **tokens,
							t_shell *shell);
t_ast					*parse_pipe(t_dll **tokens, t_shell *shell);
t_ast					*parse_simple_command(t_dll **tokens, t_shell *shell);
void					execute_simple_command(t_shell *shell);
int						execute(t_shell *shell, char *path, char *cmd, \
							char **args);
char					*get_command_path(t_shell *shell, char *cmd,
							t_env *env_list);
int						check_valid_cmd(t_shell *shell, char *cmd);
//----------------------signals---------------------//
void					setup_signal_handlers(void);
void					reset_signal_handlers(void);
int 					get_exit_code(int status);
void					save_restore_fds(int *saved_stdout, int *saved_stdin,
							int restore);

void					add_arg_to_list(t_dll **tail, t_dll *new_arg);
void					copy_token_properties(t_shell *shell, t_dll *src, t_dll *dst);

//--------------------------pipe------------------//
void					handle_child1(t_shell *shell, int pipefd[2],
							t_ast *original_ast);
void					handle_child2(t_shell *shell, int pipefd[2],
							t_ast *original_ast);
//----------------redirections-------------------------//
int						redirection(t_shell *shell, t_ast *redir);
void					execute_redirections(t_shell *shell,
							t_ast *original_ast);
#endif