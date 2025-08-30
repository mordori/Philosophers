/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:01:50 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 02:37:27 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int argc, char *argv[])
{
	t_sim	sim;

	if (argc < 5 || argc > 6)
	{
		ft_error("Invalid amount of arguments.");
		return (EXIT_FAILURE);
	}
	if (!init_sim(&sim, argc, argv))
		return (EXIT_FAILURE);
	if (!simulate(&sim))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
