/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:01:50 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 02:48:32 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "errors.h"
#include "simulation.h"
#include "simulation_utils.h"
#include "logging.h"

int	main(int argc, char *argv[])
{
	t_sim	sim;

	if (argc < 5 || argc > 6)
	{
		ft_perror("Invalid amount of arguments.");
		return (EXIT_FAILURE);
	}
	memset(&sim, 0, sizeof(sim));
	if (!init_sim(&sim, argc, argv))
		return (EXIT_FAILURE);
	simulate(&sim);
	clean_sim(&sim);
	return (EXIT_SUCCESS);
}
