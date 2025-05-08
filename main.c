/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/05/08 18:33:27 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smash.h" 

// void print_ast_tree(t_ast *node, int depth, int is_last, const char *prefix) 
// {
//     if (!node) return;

//     printf("%s", prefix);
//     printf(is_last ? "└── " : "├── ");

//     if (node->token) 
//     {
//         if (node->token->operator == AND)
//             printf("AND\n");
//         else if (node->token->operator == OR)
//             printf("OR\n");
//         else if (node->token->token_type == REDIRECTION)
//             printf("%s\n", node->token->value);
//         else if (node->token->token_type == PIPE)
//             printf("PIPE\n");
//         // else if (node->token->bracket == 1)
//         //     printf("( )\n");
//         else if (node->token->token_type == WORD)
//         {
//             printf("CMD: %s", node->token->value);
//             if(node->arguments)
//             {
//                 t_arg *arg = node->arguments;
//                 printf(" [args:");
//                 while (arg) 
//                 {
//                     printf(" %s", arg->argument);
//                     arg = arg->next;
//                 }
//                 printf("]");
//             }
//             printf("\n");
//         }   
//     }

//     char new_prefix[256];
//     snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");
//     print_ast_tree(node->left, depth + 1, node->right == NULL, new_prefix);
//     print_ast_tree(node->right, depth + 1, 1, new_prefix);
// }

// void printtt(t_ast *root)
// {
//     printf("AST Visualization:\n");
//     print_ast_tree(root, 0, 1, "");
// }

int parsing(t_shell *shell, char *input)
{
	shell->tokens = tokenize_input(shell, input);
	if (parse_input(shell, &shell->tokens))
        return (1);
    heredoc(shell, &shell->tokens);
    redirect(&shell->tokens);
    shell->ast = abstract_segment_tree(shell);
    // printtt(shell->ast);
	// exit(0);
	// t_dll   *curr;
	// t_dll *tmp;
	// tmp = shell->tokens;
	// wildcard(shell, &shell->tokens, shell->tokens);
	// while (tmp)
    // {
	// 	printf("Token --> : `%s', token type: %d\n", tmp->value, tmp->token_type);
    //     tmp = tmp->next;
    // }
    // curr = shell->tokens;
    // while (curr)
    // {
        // printf("Token --> : `%s', token type: %d\n", curr->value, curr->token_type);
        // curr = curr->next;
    // }
	// exit(0);
    // expansion(&shell->tokens, shell->env_list, shell->exit_code);
    // shell->ast = abstract_segment_tree(shell);
    // printtt(shell->ast);
    // t_dll   *curr;
    // curr = shell->tokens;
    // while (curr)
    // {
    //     printf("Token --> : `%s', redirection type: %d\n", curr->value, curr->redir_type);
    //     curr = curr->next;
    // }
    return (0);
    //free_token_list(&tokens);
}

static char	*ft_strjoin_free(t_shell *shell, char *s1, char *s2, int free1, int free2)
{
	char	*result;

	result = ft_strjoin(shell, s1, s2);
	if (free1 && s1)
		free(s1);
	if (free2 && s2)
		free(s2);
	return (result);
}

char	*get_prompt(t_shell *shell)
{
	char	cwd[1024];
	char	*line1;
	char	*line2;
	char	*username;
	char	*tmp;

	username = getenv("USER");
	if (!username)
		username = "user";
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		ft_strlcpy(cwd, "~",ft_strlen(cwd));

	line1 = ft_strdup(shell, B_GREEN);
	tmp = ft_strjoin(shell, line1, username);
	line1 = tmp;
	tmp = ft_strjoin_free(shell, line1, RESET B_WHITE"@"RESET B_BLUE "minishell" RESET ":", 1, 0);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, B_CYAN);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, cwd);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, RESET);
	line1 = tmp;
	
	line2 = ft_strdup(shell, YELLOW BOLD "➔ " RESET);

	tmp = ft_strjoin(shell, line1, "\n");
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, line2);
	
	return (tmp);
}

void read_eval_print_loop(t_shell *shell)
{
    char    *input;
    
    input = NULL;
	setup_signal_handlers();
    while (1337)
    {
		setup_signal_handlers();
        input = readline(get_prompt(shell));
        if (!input)
		{
			printf("exit\n");
			// burn_garbage(&shell);
			exit(0);
		}
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
	create_env(shell, &shell.env_list, env);
	read_eval_print_loop(&shell);
    // burn_garbage(&shell);
	return (0);
}
