
#include "../execution.h"

void	ft_putstr_fd(char const *s, int fd)
{
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}
void execute_builtin_cd(t_dll *tokens, t_env **env)
{
    char    *dir;
    t_dll   *arg_token;
    char    *cwd;

    cwd = get_env_var(*env, "PWD");
    if (cwd == NULL)
    {
        perror("cd: ");
        return;
    }
    arg_token = tokens->next;
    if (!arg_token || arg_token->token_type != WORD || !arg_token->value || 
        ft_strcmp(arg_token->value, "~") == 0 || ft_strcmp(arg_token->value, "") == 0)
    {
        dir = get_env_var(*env, "HOME");
        if (!dir || !*dir)
        {
            ft_putstr_fd("cd: HOME not set\n", 2);
            free(cwd);
            return;
        }
    }
    else if (ft_strcmp(arg_token->value, "-") == 0)
    {
        dir = get_env_var(*env, "OLDPWD");
        if (!dir || !*dir)
        {
            ft_putstr_fd("cd: OLDPWD not set\n", 2);
            free(cwd);
            return;
        }
        ft_putstr_fd(dir, 1);
        ft_putstr_fd("\n", 1);
    }
    else
    {
        dir = arg_token->value;

        if (arg_token->next && arg_token->next->token_type == WORD)
        {
            ft_putstr_fd("cd: too many arguments\n", 2);
            free(cwd);
            return;
        }
    }

    if (chdir(dir) != 0)
    {
        ft_putstr_fd("cd: ", 2);
        perror(dir);
        free(cwd);
        return;
    }
    set_env_var(env, "OLDPWD", cwd);
    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        perror("cd: ");
    }
    else
    {
        set_env_var(env, "PWD", cwd);
        free(cwd);
    }
}
