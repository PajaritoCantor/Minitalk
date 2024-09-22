/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:38:16 by jurodrig          #+#    #+#             */
/*   Updated: 2024/08/30 19:30:36 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/**
* @brief Mantiene el servidor en funcionamiento.
*
* Esta función entra en un bucle infinito donde el servidor espera
* y maneja señales de los clientes. Se utiliza 'pause()' para
* suspender la ejecución hasta que se reciba una señal.
*/

void	keep_server_up(void)
{
	while (1)
	{
		sleep(1);
	}
}

int	get_bit_value(int signum)
{
	if (signum == SIGUSR1)
		return (0);
	return (1);
}
/**
* @brief Inicializa el servidor
* y configura los manejadores de señales
*
* @return int Retorna 0 si la ejecución es exitosa.
*/
#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

int	pong(int pid)
{
	if (kill(pid, SERVER_READY) < 0)
	{
		ft_print_error("Error al enviar SERVER_READY");
		return EXIT_FAILURE;
	}
	g_client.actual_pid = pid;
	g_client.getting_header = 1;
	return (EXIT_SUCCESS);
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
