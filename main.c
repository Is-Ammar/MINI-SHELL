/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/26 01:55:14 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parsing.h" 

int	parsing(t_dll **tokens, char *input)
{
	
	*tokens = tokenize_input(input);
	if (parse_input(tokens))
		return 1;
	return (0);
	// t_dll	*curr;
	// curr = tokens;
	// while (curr)
	// {
	// 	printf("Token --> : `%s'\n", curr->value);
	// 	curr = curr->next;
	// }
	// Abstract_segment_tree(tokens);
	//free_token_list(&tokens);
}
void	read_eval_print_loop(t_env **env)
{
	t_dll	*tokens;
	char	*input;
	
	input = NULL;
	while (1337)
	{
		input = readline(RED "minishell:~$ " RESET);
		if (!input)
		{
			free(input);
			return ;
		}
		if (parsing(&tokens, input))
			continue ;
		add_history(input);
		execution(&tokens, env);
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