#include "smash.h" 

void	token_parse_exec(char *input,t_env **env)
{
	t_dll	*tokens;

	tokens = tokenize_input(input);
	if (parse_input(tokens))
		return (free_token_list(&tokens));
	// while (tokens)
	// {
	// 	printf("Token --> : `%s'\n", tokens->value);
	// 	tokens = tokens->next;
	// }
	// Abstract_segment_tree(tokens);
	execute_input(tokens, env);
	//free_token_list(&tokens);
}
void	read_eval_print_loop(t_env **env)
{
	char	*input;
	
	input = NULL;
	while (1337)
	{
		input = readline(RED "minishell:~$ " RESET);
		if (!input || !ft_strncmp(input, "exit", 4))
		{
			printf("exit\n");
			free(input);
			return ;
		}
		add_history(input);
		token_parse_exec(input, env);
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	t_env *env_var;

	if (ac != 1)
		return (1);
	env_var =  NULL;
	create_env(&env_var , env);
	read_eval_print_loop(&env_var);
	return (0);
}