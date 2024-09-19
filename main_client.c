/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:48:42 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/04 20:51:43 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

#define RETRY_TIMES 30
#define RETRY_TIME 2

#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

t_global	g_server;

void	handler(int signum)
{
	(void)signum;
}
/**
* @Envía el tamaño y el contenido del mensaje,
* desde client hasta server.
* Se comunica con el server enviando
* un flujo de señales que el servidor puede interpretar como bits,
* y luego reconstruir estos bits
* en un mensaje coherente
**/

void	ping_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum, (void)context, (void)info;
	printf("Señal recibida de PID: %d\n", info->si_pid);
	if (info->si_pid == getpid())
	{
		ft_printfd(2, "Error: Own process\n");
		exit(1);
	}
	if (info->si_pid != g_server.pid)
	{
		ft_printfd(2, "Error: Unexpected pid in ping_handler\n");
		return ;
	}
	if (signum == SERVER_READY)
	{
		g_server.is_ready = 1;
		printf("Servidor listo (SIGUSR1 recibido)\n");
	}
	if (signum == SERVER_BUSY)
	{
		g_server.is_ready = 0;
		printf("Servidor no listo (SIGUSR2 recibido)\n");
	}
}

void	handle_timeouts(int pid)
{
	int	i;

	i = -1;
	while (++i < RETRY_TIMES)
	{
		kill(pid, SIGUSR1);
		ft_printf("Esperando respuesta del servidor\n");
		sleep(RETRY_TIME);
		if (g_server.is_ready == 1)
			break ;
	}
}

int	ping(int pid)
{
	struct sigaction	sa;

	printf("Configurando ping para el servidor con PID: %d\n", pid);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = ping_handler;
	g_server.pid = pid;
	g_server.is_ready = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	handle_timeouts(pid);
	printf("Server ready: %d\n", g_server.is_ready);
	return (g_server.is_ready);
}

int	main(int argc, char **argv)
{
	int		msg_len;
	t_info	data;

	(void)argc, (void)argv;
	printf("Número de argumentos recibidos: %d\n", argc);
	parser(argc, argv);
	init_data(argv, &data);
	if (ping(data.server_pid) == 0)
	{
		printf("El servidor no está listo.\n");
		return (0);
	}
	msg_len = ft_strlen(argv[2]);
	printf("MSG_LEN: [%d]\n", msg_len);
	send_signals(&msg_len, 32, &data);
	printf("Enviando mensaje...\n");
	send_message(data.message, &data);
	printf("Mensaje enviado correctamente\n");
	return (0);
}
