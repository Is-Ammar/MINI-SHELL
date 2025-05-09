/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:40:44 by iammar            #+#    #+#             */
/*   Updated: 2025/05/09 14:29:11 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../smash.h"

void sigint_handler(int sig)
{
    (void)sig;
    g_received = SIGINT;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void sigquit_handler(int sig)
{
    (void)sig;
}

void setup_signal_handlers(void)
{
    signal(SIGQUIT, sigquit_handler);
    signal(SIGINT, sigint_handler);
}

void reset_signal_handlers(void)
{
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);
}
