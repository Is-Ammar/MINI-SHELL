/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/12 22:09:32 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smash.h" 

int parsing(t_shell *shell, char *input)
{
	shell->tokens = tokenize_input(input);
	if (parse_input(&shell->tokens))
        return (1);
    expansion(&shell->tokens, &shell->env_list, shell->exit_code);
    shell->ast = abstract_segment_tree(shell);
    printtt(shell->ast); 
    // t_dll   *curr;
    // curr = shell->tokens;
    // while (curr)
    // {
    //     printf("Token --> : `%s', token type: %d\n", curr->value, curr->operator);
    //     curr = curr->next;
    // }
    return (0);
    //free_token_list(&tokens);
}

void read_eval_print_loop(t_shell *shell)
{
    char    *input;
    
    input = NULL;
    while (1337)
    {
        input = readline(CYAN"minishell:~$ "RESET);
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
        // execution(shell);
        free(input);
    }
}

int main(int ac, char **av, char **env)
{
    (void)av;
    t_shell (shell);
    if (ac != 1)
		return (1);

	shell = (struct s_shell){0};
	create_env(&shell.env_list, env);
	read_eval_print_loop(&shell);
	// free_shell(shell);
	return (0);
}
