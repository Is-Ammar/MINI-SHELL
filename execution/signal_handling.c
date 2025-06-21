/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:40:44 by iammar            #+#    #+#             */
/*   Updated: 2025/06/21 15:33:41 by habdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../smash.h"

t_shell	*save_shell(t_shell *shell)
{
	static t_shell	*saved;

	if (shell)
		saved = shell;
	return (saved);
}

void	heredoc_handler(int sig)
{
	(void)sig;
	if (save_shell(0)->heredoc.fd > 0)
	{
		unlink(save_shell(0)->heredoc.name);
		close(save_shell(0)->heredoc.fd);
	}
	burn_garbage(save_shell(0));
	exit(130);
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_received = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
}

void	reset_signal_handlers(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}
