/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 04:14:27 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/20 04:14:35 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

int	pong(int pid)
{
	kill(pid, SERVER_READY);
	g_client.actual_pid = pid;
	g_client.getting_header = 1;
	return (EXIT_SUCCESS);
}

