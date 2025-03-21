
#include "../execution.h"

void	execute_builtin_pwd(t_env *env)
{
	char	*cwd;

	cwd = get_env_var(env, "PWD");
	if (cwd)
	{
		printf("%s\n", cwd);
	}
}
