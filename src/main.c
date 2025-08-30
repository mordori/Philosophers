/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:01:50 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 20:51:42 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "errors.h"
#include "simulation.h"
#include "simulation_utils.h"

int	main(int argc, char *argv[])
{
	t_sim	sim;
	bool	result;

	if (argc < 5 || argc > 6)
	{
		ft_perror("Invalid amount of arguments.");
		return (EXIT_FAILURE);
	}
	if (!init_sim(&sim, argc, argv))
		return (EXIT_FAILURE);
	result = simulate(&sim);
	free_sim(&sim);
	return (!result);
}
