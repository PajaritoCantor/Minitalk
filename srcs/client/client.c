/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:24:29 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/30 21:32:50 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	init_client_data(char **argv, t_info *data)
{
	ft_memset(data, 0, sizeof(t_info));
	data->server_pid = ft_atoi_limits(argv[1]);
	data->client_pid = getpid();
	ft_printfd(2, "PID CLIENT %d\n", data->client_pid);
	data->msg = argv[2];
	if (data->server_pid == 0)
		ft_print_error(BAD_SIGNAL);
}

void	client_signal_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum, (void)context, (void)info;
}

void	send_signal(pid_t pid, int signal)
{
	if (kill(pid, signal))
		ft_print_error("Signal sending failed.");
}

void	send_message_bits(void *data, size_t bit_length, t_info *info)
{
	unsigned long long	value;
	int					i;

	value = 0;
	if (bit_length == 8)
		value = *((unsigned char *)data);
	else if (bit_length == 32)
		value = *((unsigned int *)data);
	i = bit_length - 1;
	while (i >= 0)
	{
		if (value & (1ULL << i))
			send_signal(info->server_pid, CHAR_1);
		else
			send_signal(info->server_pid, CHAR_0);
		i--;
		usleep(500 * 500);
	}
}

void	transmit_message_to_server(char *str, t_info *data)
{
	struct sigaction	sa;
	int					i;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = client_signal_handler;
	sigaction(SIGUSR2, &sa, NULL);
	i = 0;
	while (str[i])
		send_message_bits(&str[i++], 8, data);
}
