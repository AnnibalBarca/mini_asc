/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_setup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nagaudey <nagaudey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:41:25 by almeekel          #+#    #+#             */
/*   Updated: 2025/07/24 15:37:19 by nagaudey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	setup_signal(int signo, void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	if (handler == SIG_DFL || handler == SIG_IGN)
		sa.sa_flags = SA_RESTART;
	else
		sa.sa_flags = 0;
	if (sigaction(signo, &sa, NULL) != 0)
		ft_putstr_fd("minishell: signal setup failed\n", 2);
}

void	setup_child_signals(void)
{
	setup_signal(SIGINT, SIG_DFL);
	setup_signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_signals(void)
{
	setup_signal(SIGINT, SIG_IGN);
	setup_signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signals(void)
{
	setup_signal(SIGINT, handle_sigint_heredoc);
	rl_event_hook = handle_event_hook;
}

void	setup_postheredoc_signals(void)
{
	setup_signal(SIGINT, handle_sigint);
	rl_event_hook = NULL;
}

static void	handle_sigquit_noninteractive(int sig)
{
	(void)sig;
	g_signal_status = 131;
}

static void	handle_sigint_noninteractive(int sig)
{
	(void)sig;
	g_signal_status = 130;
}

void	setup_noninteractive_signals(void)
{
	setup_signal(SIGINT, handle_sigint_noninteractive);
	setup_signal(SIGQUIT, handle_sigquit_noninteractive);
}
