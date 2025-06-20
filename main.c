/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/20 08:13:37 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smash.h"

int		g_received = 0;

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
		if (g_received == SIGINT)
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
			free(input);
			printf("exit\n");
			rl_clear_history();
			clean_exit(shell, 0);
		}
		else if (check_input(shell, input))
			continue ;
		add_history(input);
		execution(shell);
		clear_non_env(shell);
		free(input);
	}
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
	t_shell	shell;

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
