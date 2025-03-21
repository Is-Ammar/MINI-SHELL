
#include "../execution.h"

size_t	ft_strlcpy(char *dst, char *src, size_t len)
{
	size_t	i;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (len == 0)
		return (src_len);
	i = 0;
	while (src[i] && i < len - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}
void create_env(t_env **env, char **environ)
{
    int i;
    char *equals_sign;
    char *name;
    char *value;
    int name_len;

    i = 0;
    *env = NULL;

    while (environ[i])
    {
        equals_sign = ft_strchr((const char *)environ[i], '=');
        if (equals_sign)
        {
            name_len = equals_sign - environ[i];
            name = malloc(name_len + 1);
            if (!name)
                continue;
            ft_strlcpy(name, environ[i], name_len + 1);
            name[name_len] = '\0';
            value = ft_strdup(equals_sign + 1);
            if (!value)
            {
                free(name);
                continue;
            }
            add_env_var(env, name, value);
            free(name);
            free(value);
        }
        i++;
    }
}
