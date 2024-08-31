/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:27:19 by jurodrig          #+#    #+#             */
/*   Updated: 2024/08/31 06:09:36 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_global g_server;

/**
* @brief Reserva memoria para almacenar el mensaje 
* basándose en el tamaño recibido.

* Asigna memoria para el mensaje según el tamaño indicado 
* por g_server.msg_size.

* Si la asignación falla, imprime un mensaje
* de error y sale del programa.
*/
void	reserve_memory_for_msg(void)
{
	g_server.msg = (char *)ft_calloc(g_server.msg_size + 1, sizeof(char));
	if (!g_server.msg)
	{
		perror("Malloc allocation failed");
		exit(EXIT_FAILURE);
	}
}

/**
* @brief Maneja el escenario de ṕerdida de señal.
*
* Verifica si se perdió una señal y reasigna el PID
* del cliente actual del servidor si es necesario
*
* @param s_si_pid El PID del cliente guardado en el servidor.
* @param signum La señal recibida.
* @param context Puntero de conteto no utilizado.
* @return El PID del cliente actualizado
*/
int	lost_signal(int sender_pid, int signum, int *i, void *context)
{
	(void)context;

	if (sender_pid == 0 && (signum == SIGUSR1 || signum == SIGUSR2))
	{
		printf("i [%d] client: %d con señal: %d\n", (*i), sender_pid, signum);
		sender_pid = g_server.actual_pid;
	}
	return (0);
}

/**
* @brief Maneja la recepción del encabezado (tamaño del mensaje).
*
* Procesa los bits recibidos como parte del encabezado del mensaje
* y reserva memoria una vez que se ha recibido el encabezado completo.
*
* @param i El índice actual del bit.
* @para signum La señal recibida.
*/
void	handle_header(int *i, int signum)
{
	int	bit_value;

	bit_value = get_bit_value(signum);
	if (*i < HEADER_SIZE)
	{
		g_server.msg_size |= (bit_value << (HEADER_SIZE - 1 - *i));
		(*i)++;
	}
	if (*i == HEADER_SIZE)
	{
		reserve_memory_for_msg();
	}
}

/**
* @brief Maneja la recepción del mensaje.
*
* Ensambla el mensaje a partir de los bits recibidos y lo almacena en memoria.
* 
* Una vez que se recibe el mensaje completo, 
* se imprime y se reinicia el servidor.
*
* @param i EL índice actual del bit
* @param signum La señal recibida.
*/
void	handle_msg(int *i, int signum)
{
	static char	char_value;
	static int	msg_pos;
	int	bit_value;

	bit_value = get_bit_value(signum);
	char_value |= (bit_value << (7 - (*i % 8)));
	(*i)++;
	if (*i % 8 == 0 && *i != 0)
	{
		g_server.msg[msg_pos] = char_value;
		msg_pos++;
		char_value = 0;
	}
	if (msg_pos == g_server.msg_size)
	{
		printf("message: [%s]\n", g_server.msg);
		free(g_server.msg);
		ft_bzero(&g_server, sizeof(g_server));
		g_server.getting_header = 1;
		*i = 0;
		msg_pos = 0;
	}
}
/**
* @brief Manejador de señales para el servidor.
*
* Este manejador procesa las señales recibidas del cliente,
* actualiza el estado del servidor 
* y ensambla el mensaje a partir de las señales.
*
* @param signum La señal recibida.
* @param info Información adicional sobre la señal y el proceso que la envió.
* @param context Puntero de contexto no utilizado.
*/

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static int	i;

	(void)context;
	i = 0;
	if (!g_server.actual_pid)
		g_server.actual_pid = info->si_pid;
	if (info->si_pid != g_server.actual_pid || info->si_pid == getpid())
		return ;
	if (g_server.getting_header)
		handle_header(&i, signum);
	else
		handle_msg(&i, signum);
	if (i % 8 == 0 && !g_server.getting_header && !g_server.getting_msg)
		kill(g_server.client_pid, SIGNAL_RECEIVED);
}
