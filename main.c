/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/14 16:56:18 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smash.h" 

int g_received = 0;


 void print_ast_tree(t_ast *node, int depth, int is_last, const char *prefix) 
 {
	 if (!node) return;
 
	 printf("%s", prefix);
	 printf(is_last ? "└── " : "├── ");
 
	 if (node->token) 
	 {
		 if (node->token->operator == AND)
			 printf("AND\n");
		 else if (node->token->operator == OR)
			 printf("OR\n");
		 else if (node->token->token_type == PIPE)
			 printf("PIPE\n");
		 else if (node->token->redir_type != 0)
		 {
			 if (node->token->redir_type == READ)
				 printf("REDIR: < (input from %s)\n", node->token->value);
			 else if (node->token->redir_type == WRITE)
				 printf("REDIR: > (output to %s)\n", node->token->value);
			 else if (node->token->redir_type == APPEND)
				 printf("REDIR: >> (append to %s)\n", node->token->value);
			 else
				 printf("REDIR: unknown type (%s)\n", node->token->value);
		 }
		 else if (node->token->token_type == WORD)
		 {
			 printf("CMD: %s", node->token->value);
			 t_dll *arg = node->arguments;
			 while (arg)
			 {
				 printf(" arg: %s ", arg->value);
				 arg = arg->next;
			 }
			 printf("\n");
		 }   
	 }
	 else
	 {
		 printf("EMPTY NODE\n");
	 }
 
	 char new_prefix[256];
	 snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");
	 print_ast_tree(node->left, depth + 1, node->right == NULL, new_prefix);
	 print_ast_tree(node->right, depth + 1, 1, new_prefix);
 }
 
 void printtt(t_ast *root)
 {
	 printf("AST Visualization:\n");
	 print_ast_tree(root, 0, 1, "");
 }


int parsing(t_shell *shell, char *input)
{
	shell->tokens = tokenize_input(shell, input);
	if (parse_input(shell, &shell->tokens))
		return (1);
	heredoc(shell, &shell->tokens);
	if (g_received == SIGINT)
		return (1);
	last_check_doc(&shell->tokens);
	redirect(&shell->tokens);
	shell->ast = abstract_segment_tree(shell);
	// printf("token : %s | type: %d\n",shell->tokens->value, shell->tokens->token_type);
	/* t_dll *curr = shell->tokens;
	t_dll	*nxt; */
	// expansion(shell, &shell->tokens, &shell->tokens);
	// printf("token : %s | type: %d\n",curr->value,curr->token_type);
	// curr = shell->ast->arguments;
	// while (curr)
	// {
	// 	nxt = curr->next;
	// 	expansion(shell, &shell->tokens, &curr);
	// 	curr = nxt;
	// }
	// curr = shell->tokens;
	// while (curr)
	// {
	// 	printf("value : %s | type: %d\n",curr->value,curr->token_type);
	// 	curr = curr->next;
	// }
	// exit(0);
	// expand_heredoc(shell, shell->tokens->value);
	#ifdef DEBUG
    printtt(shell->ast);
	exit(0);
	#endif
	// t_dll   *curr;
	// t_dll *tmp;
	// tmp = shell->tokens;
	// wildcard(shell, &shell->tokens, shell->tokens);
	// while (tmp)
    // {
	// 	if (check_depth_to_expand(tmp->value))
	// 		tmp->wildcard = TRUE;
    //     tmp = tmp->next;
    // }
    // curr = shell->tokens;
    // while (curr)
    // {
    //     printf("Token --> : `%s', token is wildcard: %d\n", curr->value, curr->wildcard);
    //     curr = curr->next;
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
	tmp = ft_strjoin(shell, line1, RESET B_WHITE"@"B_GRAY"minishell"B_WHITE" ❯ ");
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, B_BLUE "┃" B_CYAN BG_BLACK);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, cwd);
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, RESET B_BLUE "┃" RESET);
	line1 = tmp;
	line2 = ft_strdup(shell, B_YELLOW "\001➔\002 \x7f" RESET);
	tmp = ft_strjoin(shell, line1, "\n");
	line1 = tmp;
	tmp = ft_strjoin(shell, line1, line2);
	return (tmp);
}

int	check_input(t_shell *shell, char *input)
{
	if (!*input)
	{
		shell->exit_code = 0;
		return (1);
	}
	if (g_received == SIGINT)
	{
		shell->exit_code = 130;
		g_received = 0;
	}
	if (parsing(shell, input))
	{
		if(g_received == SIGINT)
		{
			shell->exit_code = 130;
			g_received = 0;
		}
		else
			shell->exit_code = 2;
		add_history(input);
		return (1);
	}
	return (0);
}

void	read_eval_print_loop(t_shell *shell)
{
	char	*input;
	
	input = NULL;
	while (1337)
	{
		setup_signal_handlers();
		input = readline(get_prompt(shell));
		shell->lines++;
		if (!input)
		{
			printf("exit\n");
			free(input);
			rl_clear_history();
			clean_exit(shell, 0);
		}
		if (check_input(shell, input))
			continue ;
		add_history(input);
		execution(shell);
		clear_non_env(shell);
		free(input);
	}
	printf("exit\n");
	clean_exit(shell, shell->exit_code);
}

void	non_interactive_mode(t_shell *shell)
{
	char	*input;

	input = NULL;
	while (1337)
	{
		input = my_readline(shell);
		shell->lines++;
		if (!input)
			clean_exit(shell, shell->exit_code);
		if (parsing(shell, input))
		{
			shell->exit_code = 2;
			clean_exit(shell, 2);
		}
		execution(shell);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell shell;
	(void)av;
	(void)ac;

	memset(&shell, 0, sizeof(shell));
	create_env(&shell, &shell.env_list, env);
	shell.interactive = isatty(STDIN_FILENO);
	if (shell.interactive)
		read_eval_print_loop(&shell);
	else
		non_interactive_mode(&shell);
	return (0);
}
