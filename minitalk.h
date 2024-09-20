/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:26:28 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/05 00:03:57 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# define CHAR_0 SIGUSR1
# define CHAR_1 SIGUSR2

# include "library/libft.h"
# include <signal.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <ctype.h>

# define USAGE "\rUsage:\n\t./client [pid_server] [message]"

# define HEADER_SIZE	32
# define SIGNAL_RECEIVED SIGUSR2

//server
typedef struct s_msg
{
	int		size_msg;
	char	*message;
}	t_msg;

typedef struct s_global
{
	int						pid;
	int						client_pid;
	int						actual_pid;
	int						getting_header;
	int						getting_msg;
	t_msg					msg;
	volatile sig_atomic_t	is_ready;
}	t_global;

extern t_global					g_client;
//client
typedef struct s_info
{
	pid_t	client_pid;
	pid_t	server_pid;
	char	*message;
}	t_info;

extern t_global					g_server;

// client
void	init_data(char **argv, t_info *data);
void	send_signal(pid_t pid, int signal);
void	send_signals(void *data, size_t bit_length, t_info *info);
void	send_message(char *str, t_info *data);
void	signal_handler(int signum, siginfo_t *info, void *context);
// server
void	reserve_memory_for_msg(int *i);
int		lost_signal(int sender_pid, int signum, int *i, void *context);
void	handle_header(int *i, int signum);
void	handle_msg(int *i, int signum);
void	handle_timeouts(int pid);
void	parser(int argc, char **argv);
void	evaluate(const char *str);
void	keep_server_up(void);
int		get_bit_value(int signum);
int		pong(int pid);

#endif
