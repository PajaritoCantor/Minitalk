/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:53:49 by psegura-          #+#    #+#             */
/*   Updated: 2024/09/25 00:11:58 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_global	g_client;

void	keep_server_up(void)
{
	while (1)
	{
		sleep(1);
	}
}

int	lost_signal(int s_si_pid, int signum, int *i, void *context)
{
	(void)context;
	if (s_si_pid == 0 && (signum == SIGUSR1 || signum == SIGUSR2))
	{
		ft_printf("i: [%d] client: %d with signal: %d\n", (*i), s_si_pid, signum);
		s_si_pid = g_client.actual_pid;
	}
	return (s_si_pid);
}

void	server_signal_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	static int	i;

	info->si_pid = lost_signal(info->si_pid, signum, &i, context);
	if (info->si_pid == getpid())
		ft_print_error("Own process");
	g_client.client_pid = info->si_pid;
	if (g_client.actual_pid == 0)
	{
		toc(g_client.client_pid);
		return ;
	}
	if (g_client.actual_pid != g_client.client_pid)
		return ;
	if (g_client.getting_header == 1)
		handle_header(&i, signum);
	else if (g_client.getting_msg == 1)
		handle_msg(&i, signum);
	if (g_client.client_pid != 0 && (signum == SIGUSR1 || signum == SIGUSR2))
		kill(g_client.client_pid, SIGNAL_RECEIVED);
}

int	main(void)
{
	struct sigaction	sa;
	pid_t				server_pid;

	ft_memset(&g_client, 0, sizeof(t_global));
	server_pid = getpid();
	ft_printfd(1, "Server PID: %d\n", server_pid);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = server_signal_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	keep_server_up();
	return (0);
}
