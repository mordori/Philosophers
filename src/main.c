/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:01:50 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 13:51:21 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "errors.h"
#include "simulation.h"
#include "simulation_utils.h"
#include "logging.h"

/**
 * @brief Classic dining philosophers problem with threads and mutex.
 *
 * Philosophers think, eat, and sleep. Each philosopher is represented by a
 * thread. There is one fork per philosopher, but in order to eat a philosopher
 * needs two. So they have to take turns in eating. None should die from
 * starvation if the provided parameters consider the combined time of the
 * actions. The philosophers are not allowed to communicate with each other.
 *
 * Resources shared between threads are locked with mutex to avoid data races.
 *
 * Main thread handles monitoring whether the philosophers are alive, and
 * if they have eaten the optionally provided number of meals. In those cases
 * the simulation will be flagged to be over.
 *
 * A dedicated logging thread that is decoupled from philosopher status
 * monitoring handles printing the states of philosophers to STDOUT. It
 * constructs batches from a queue of logs and writes them out periodically.
 *
 * Expects 4 arguments, with optional 5th one in the form
 *
 * ./philo [number of philos] [time to die] [time to eat] [time to sleep]
 * (optional)[number of meals].
 *
 * For example:
 *
 * - ./philo 1 100 100 100
 *
 * - ./philo 3 300 100 200
 *
 * - ./philo 199 180 60 60 5
 *
 * - ./philo 200 125 60 60 10
 *
 * @param argc Number of program arguments.
 * @param argv Array containing the program arguments.
 */
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
