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
		pause();
}

/**
* @brief Inicializa el servidor
* y configura los manejadores de señales
*
* @return int Retorna 0 si la ejecución es exitosa.
*/
int	main(void)
{
	struct sigaction	sa;

	ft_memset(&g_server, 0, sizeof(t_global));
	printf("Server PID: %d\n", getpid());
	sa.sa_sigaction = &signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	keep_server_up();
	return (0);
}
