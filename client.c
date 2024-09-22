/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:23:24 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/05 00:02:49 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/**
* @brief inicializa la struct de datos t_info
* con la información del cliente y servidor.
* @param argv Argumentos del programa, incluyendo PID del servidor y mensaje.
* @param data Puntero a la estructura t_info a inicializar.
* @Inicializa los datos.
* @Convierte el PID del servidor.
* @Obtiene el PID del cliente actual.
* @Asigna el mensaje de los argumentos.
* @Verifica que el PID del servidor sea válido.
*/

void	init_data(char **argv, t_info *data)
{
	ft_memset(data, 0, sizeof(t_info));
	data->server_pid = ft_atoi_limits(argv[1]);
	data->client_pid = getpid();
	ft_printfd(2, "PID CLIENT %d\n", data->client_pid);
	data->message = argv[2];
	if (data->server_pid == 0)
		ft_print_error("BAD_SIGNAL");
}

/**
 * @brief Manejador de señales. Se activa cuando se recibe una señal.
 * @param signum = Señal recibida
 * @param info = Información sobre el proceso que envió la señal.
 * @param context = Contexto del manejador de señal.
 *
 * Este manejador de señales se activa
 * cuando se recibe una señal específica. */
 
void	client_signal_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)info;
	(void)context;
}
/*
* @brief Envía una señal a un proceso específico utilizando 'kill()'.
* @param pid = El PID del proceso destino
* @param signal = Señal que se desea enviar (SIGUSR1 o SIGUSR2).
*
* Esta función envía una señal a un proceso utilizando la llamada 'kill()'. 
* Si ocurre un error al enviar la señal, 
* muestra un mensaje de error y termina el programa. 
* La información sobre el envío se imprime en la consola.
*/

void	send_signal(pid_t pid, int signal)
{
	if (kill(pid, signal))
	{
		perror("Error al enviar la señal.");
		exit(EXIT_FAILURE);
	}
}

/*
* @brief envía los bits de un valor como señales al servidor.
*
* Esta función toma un valor (de 8 o 32 bits) y lo envía al servidor bit a bit.
* Cada bit se envía como una señal 'SIGUSR1' (para un bit 1) 
* o 'SIGUSR2' (para un bit 0).
* Después de enviar cada bit 
* se usa 'usleep' para pausar brevemente 
* y darle tiempo al servidor para procesar la señal.
*
* @param value Valor a enviar como señales
* @param bit_length Longitud en bits del valor a enviar.
* @param info Puntero a la estructura t_info con la información del servidor.
*/

void	send_signals(void *data, size_t bit_length, t_info *info)
{
	int					i;
	unsigned long long	value;

	value = 0;
	if (bit_length == 8)
		value = *((unsigned char *)data);
	else if (bit_length == 32)
		value = *((unsigned int *)data);
	i = bit_length - 1;
	while (i >= 0)
	{
		if (value & (1ULL << i))
		{
			send_signal(info->server_pid, CHAR_1);
		}
		else
		{
			send_signal(info->server_pid, CHAR_0);
		}
		i--;
		usleep (10000);
	}
}

/**
* @brief Envía un mensaje al servidor, bit a bit
*
* Esta función se encarga de enviar el mensaje completo al servidor.
* Primero, se envía la longitud del mensaje en 32 bits. Luego, se envía cada
* carácter del mensaje en su representación de 8 bits.
*
* @param data Puntero a la estructura t_info que contiene el mensaje y PIDs.
*/
void	send_message(char *str, t_info *data)
{
	struct sigaction	sa;
	int					i;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = client_signal_handler;
	sigaction(SIGUSR2, &sa, NULL);
	i = 0;
	while (str[i])
		send_signals(&str[i++], 8, data);
}
