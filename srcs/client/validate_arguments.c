/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:24:51 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/28 02:17:06 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int	get_state(int x, int y)
{
	const int	states[][2] = {\
	{1, 2}, \
	{1, 1}, \
	{1, 2}, \
	};

	return (states[x][y]);
}

int	choose_state(int state, char c)
{
	int	pos;

	pos = 0;
	if (ft_isdigit(c))
		pos = 1;
	state = get_state(state, pos);
	return (state);
}

void	evaluate_pid(const char *str)
{
	int	i;
	int	state;

	i = 0;
	state = 0;
	while (str[i])
	{
		state = choose_state(state, str[i]);
		i++;
	}
	if (state < 2)
		ft_print_error((char *)str);
}

void	validate_arguments(int argc, char **argv)
{
	if (argc != 3)
		ft_print_error(USAGE);
	evaluate_pid(argv[1]);
}
