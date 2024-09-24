/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:56:37 by psegura-          #+#    #+#             */
/*   Updated: 2024/09/24 20:25:31 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

int	toc(int pid)
{
	kill(pid, SERVER_READY);
	g_client.actual_pid = pid;
	g_client.getting_header = 1;
	return (EXIT_SUCCESS);
}
