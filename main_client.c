/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:48:42 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/04 20:22:50 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
void    handler(int signum)
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
void	send_bit(int bit, int server_pid)
{
	if (bit)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
	usleep(100);
}
int	main(int argc, char *argv[])
{
	pid_t	pid;
	char *message;
	struct sigaction sa;

	if (argc != 3)
	{
		printf("Uso: %s <PID_servidor> <mensaje>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	pid = ft_atoi(argv[1]);
	message = argv[2];

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);

	send_message(pid, message);

	return (EXIT_SUCCESS);
}
