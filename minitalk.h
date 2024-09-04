/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:26:28 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/04 20:18:32 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include "library/libft.h"
# include <signal.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <ctype.h>

#define HEADER_SIZE 32
#define SIGNAL_RECEIVED SIGUSR2
# define CHAR_0 SIGUSR1
# define CHAR_1 SIGUSR2

typedef struct s_global
{
	int		actual_pid;
	pid_t	client_pid;
	int		msg_size;
	int		getting_header;
	int		getting_msg;
	char	*msg;
}	t_global;

typedef struct s_info
{
	pid_t	client_pid;
	pid_t	server_pid;
	char	*msg;
	int	msg_len;
}	t_info;

extern t_global g_server;
// client
void	init_data(char **argv, t_info *data);
void	send_signal(pid_t pid, int signal);
void	send_signals(void *data, size_t bit_length, t_info *info);
void	send_message(int server_pid, const char *msg);

// server
void	reserve_memory_for_msg(void);
int		lost_signal(int sender_pid, int signum, int *i, void *context);
void	handle_header(int *i, int signum);
void	handle_msg(int *i, int signum);
void	signal_handler(int signum, siginfo_t *info, void *context);
void	keep_server_up(void);

void	send_bit(int bit, int server_pid);
#endif
