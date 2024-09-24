/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 16:23:44 by psegura-          #+#    #+#             */
/*   Updated: 2024/09/24 23:11:34 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define HEADER_SIZE 32
# define SIGNAL_RECEIVED SIGUSR2

typedef struct s_msg
{
	int		size_message;
	char	*message;
}	t_msg;

typedef struct s_global
{
	int		client_pid;
	int		actual_pid;
	int		getting_header;
	int		getting_msg;
	t_msg	msg;
}	t_global;

extern t_global	g_client;

int		toc(int pid);
void	handle_header(int *i, int signum);
void	handle_msg(int *i, int signum);

#endif
