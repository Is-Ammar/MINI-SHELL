/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/04/16 10:40:07 by habdella         ###   ########.fr       */
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
	shell->tokens = tokenize_input(input);
	if (parse_input(&shell->tokens))
        return (1);
    expansion(&shell->tokens, shell->env_list, shell->exit_code);
    heredoc(&shell->tokens, shell->env_list, shell->exit_code);
    redirections(&shell->tokens);
    shell->ast = abstract_segment_tree(shell);
    // printtt(shell->ast);
    // t_dll   *curr;
    // curr = shell->tokens;
    // while (curr)
    // {
    //     printf("Token --> : `%s', token type: %d\n", curr->value, curr->operator);
    //     curr = curr->next;
    // }
    // expansion(&shell->tokens, shell->env_list, shell->exit_code);
    // shell->ast = abstract_segment_tree(shell);
    // printtt(shell->ast);
    t_dll   *curr;
    curr = shell->tokens;
    while (curr)
    {
        printf("Token --> : `%s', redirection type: %d\n", curr->value, curr->redir_type);
        curr = curr->next;
    }
    return (0);
    //free_token_list(&tokens);
}

void read_eval_print_loop(t_shell *shell)
{
    char    *input;
    
    input = NULL;
    while (1337)
    {
        input = readline(CYAN "minishell:~$ "RESET);
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
