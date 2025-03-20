/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/03/20 19:17:22 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../smash.h"

# include <stdio.h> 
# include <stdlib.h>
# include <unistd.h>


typedef struct s_env
{
	char			*env_name;
	char			*env_value;
	struct s_env	*next;
}	t_env;

typedef struct s_dll t_dll;
//------------------------------Execution--------------------//
int	    is_builtin( char *cmd);
void execute_builtin(t_dll *tokens, t_env **env);
void	execute_input(t_dll *tokens);



//------------------------buitlin-----------------------------//

void execute_builtin_cd(t_dll *tokens, t_env **env);

//-----------------------Utils-------------------------------//
int	    ft_strcmp(char *s1, char *s2);
void	ft_putstr_fd(char const *s, int fd);
char	*ft_strdup(const char *str);


//---------------------Env-settings-------------------------//
void free_env_list(t_env *env_list);
const char *get_env_var(t_env *env_list,char *name);
void set_env_var(t_env **env_list, char *name, char *value);
t_env *create_env_var(const char *name, const char *value);

#endif