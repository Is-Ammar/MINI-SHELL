/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/03/28 23:27:19 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smash.h" 

int parsing(t_shell *shell, char *input)
{
	shell->tokens = tokenize_input(input);
	if (parse_input(&shell->tokens))
        return 1;
    return (0);
    // t_dll   *curr;
    // curr = tokens;
    // while (curr)
    // {
    //     printf("Token --> : `%s'\n", curr->value);
    //     curr = curr->next;
    // }
    // Abstract_segment_tree(tokens);
    //free_token_list(&tokens);
}

void read_eval_print_loop(t_shell *shell)
{
    char    *input;
    
    input = NULL;
    while (1337)
    {
        input = readline(BLUE "minishell:~$ " RESET);
        if (!input)
            return;
		if (!*input)
		{
			shell->exit_code = 0;
			continue;
		}
		if (parsing(shell, input))
        {
			shell->exit_code = 2;
            continue;
        }
        add_history(input);
        execution(shell);
        printf("exit code: %d\n",shell->exit_code);
        free(input);
    }
}

int main(int ac, char **av, char **env)
{
    (void)av;
    t_shell shell;

    if (ac != 1)
        return (1);
    shell = (struct s_shell){0};
    create_env(&shell.env_list, env);
    read_eval_print_loop(&shell);
    // free_shell(shell);
    return (0);
}
