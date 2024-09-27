/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:24:51 by jurodrig          #+#    #+#             */
/*   Updated: 2024/09/28 01:12:27 by jurodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int	determinate_transition(int x, int y)
{
	const int	states[][2] = {\
	{1, 2}, \
	{1, 1}, \
	{1, 2}, \
	};

	return (states[x][y]);
}

int	calculate_next_state(int state, char c)
{
	int	pos;

	pos = 0;
	if (ft_isdigit(c))
		pos = 1;
	state = determinate_transition(state, pos);
	return (state);
}

void	validate_input(const char *str)
{
	int	i;
	int	state;

	i = 0;
	state = 0;
	while (str[i])
	{
		state = calculate_next_state(state, str[i]);
		i++;
	}
	if (state < 2)
		ft_print_error((char *)str);
}

void	process_input(int argc, char **argv)
{
	if (argc != 3)
		ft_print_error(USAGE);
	validate_input(argv[1]);
}
