/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_settings1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by iammar            #+#    #+#             */
/*   Updated: 2025/04/29 21:37:42 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../smash.h"

t_env *find_env_var(t_env *env_list, char *name)
{
    t_env *current;

    current = env_list;
    while (current)
    {
        if (ft_strcmp(current->env_name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void create_env(t_shell shell, t_env **env, char **environ)
{
    int     i;
    char    *equals_sign;
    char    *name;
    char    *value;
    int     name_len;
    char   *shlvl_entry;
    int     shlvl_value;
    char    *new_shlvl;

    i = 0;
    *env = NULL;
    while (environ[i])
    {
        equals_sign = ft_strchr((const char *)environ[i], '=');
        if (equals_sign)
        {
            name_len = equals_sign - environ[i];
            name = ft_malloc(&shell, name_len + 1);
            ft_strlcpy(name, environ[i], name_len + 1);
            name[name_len] = '\0';
            value = ft_strdup(&shell, equals_sign + 1);
            add_env_var(&shell, env, name, value);
        }
        i++;
    }

    shlvl_entry = get_env_var(&shell, *env, "SHLVL");
    if (!shlvl_entry)
    {
        set_env_var(&shell, env, ft_strdup(&shell, "SHLVL"), ft_strdup(&shell, "1"));
    }
    else
    {
        shlvl_value = ft_atoi(shlvl_entry);
        if (shlvl_value < 0)
            shlvl_value = 0;
        else if (shlvl_value >= 999)
        {
            ft_putstr_fd("minishell: warning: shell level too high, resetting to 1\n",2);
            shlvl_value = 0;
        }
        shlvl_value++;
        new_shlvl = ft_itoa(&shell, shlvl_value);
        if (new_shlvl)
        {
            set_env_var(&shell, env, ft_strdup(&shell, "SHLVL"), new_shlvl);
        }
    }
}


void unset_env_var(t_shell *shell, t_dll *token)
{
    t_env *env;
    t_env *prev;

    env = shell->env_list;
    prev = NULL;

    while (env)
    {
        if (ft_strcmp(env->env_name, token->value) == 0)
        {
            if (prev)
                prev->next = env->next;
            else
                shell->env_list = env->next;
            
            free(env->env_name);
            free(env->env_value);
            free(env);
            return;
        }
        prev = env;
        env = env->next;
    }
}


char *get_current_dir_safe(t_shell *shell)
{
    char *cwd;
    char *pwd;
    
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        if (errno == ENOENT)
        {
            pwd = get_env_var(shell, shell->env_list, "PWD");
            if (pwd)
                return ft_strdup(shell, pwd);
        }
        return NULL;
    }
    return cwd;
}

char **convert_env_to_array(t_shell *shell, t_env *env_list)
{
    t_env *current;
    char **env_array = {NULL};
    int count;
    int i;
    size_t len;
    
    count = 0;
    i = 0;
    current = env_list;
    while (current)
    {
        count++;
        current = current->next;
    }
    env_array = ft_malloc(shell, (count + 1) * sizeof(char *));
    current = env_list;
    while (current)
    {
        len = ft_strlen(current->env_name) + ft_strlen(current->env_value) + 2;

        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            while (--i >= 0)
                free(env_array[i]);
            free(env_array);
            return NULL;
        }
        ft_strlcpy(env_array[i], current->env_name, ft_strlen(current->env_name) + 1);
        ft_strcat(env_array[i], "=");
        ft_strcat(env_array[i], current->env_value);

        i++;
        current = current->next;
    }

    env_array[i] = NULL;
    return env_array;
}
