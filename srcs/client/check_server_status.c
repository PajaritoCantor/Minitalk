/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_server_status.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 03:42:22 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/30 19:17:23 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

#define RETRY_TIMES 30
#define RETRY_TIME 2

#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

t_global	g_server;

void	server_state_signal_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum, (void)context, (void)info;
	if (info->si_pid == getpid())
	{
		ft_printfd(2, "Error: Own process\n");
		exit(1);
	}
	if (info->si_pid != g_server.pid)
	{
		ft_printfd(2, "Error: Unexpected Sending process ID\n");
		return ;
	}
	if (signum == SERVER_READY)
		g_server.is_ready = 1;
	if (signum == SERVER_BUSY)
		g_server.is_ready = 0;
}

void	attempt_server_connection(int pid)
{
	int	i;

	i = -1;
	while (++i < RETRY_TIMES)
	{
		kill(pid, SIGUSR1);
		ft_printf("Waiting_server_response:\n");
		sleep(RETRY_TIME);
		if (g_server.is_ready == 1)
			break ;
	}
}

int	check_server_status(int pid)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = server_state_signal_handler;
	g_server.pid = pid;
	g_server.is_ready = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	attempt_server_connection(pid);
	ft_printf("Server ready: (%d)\n", g_server.is_ready);
	return (g_server.is_ready);
}
