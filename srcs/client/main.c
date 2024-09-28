/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:53:52 by psegura-          #+#    #+#             */
/*   Updated: 2024/09/28 03:55:35 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int	main(int argc, char **argv)
{
	t_info	client;
	int		msg_len;

	(void)argc, (void)argv;
	ft_printfd(2, "ARGC:[%d]\n", argc);
	validate_arguments(argc, argv);
	init_client_data(argv, &client);
	if (check_server_status(client.server_pid) == 0)
		return (0);
	msg_len = ft_strlen(argv[2]);
	ft_printf("MSG_LEN: [%d]\n", msg_len);
	send_message_bits(&msg_len, 32, &client);
	transmit_message_to_server(client.msg, &client);
	return (0);
}
