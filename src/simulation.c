/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/20 22:05:36 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "errors.h"
#include "philosopher.h"
#include "parsing.h"
#include "simulation_utils.h"
#include "logging.h"
#include "timing.h"

static inline void	monitor_philos(t_sim *sim);
static inline bool	init_config(t_sim *sim, const int argc, char *argv[]);
static inline bool	init_mutex(t_sim *sim);

/**
 * @brief Initializes the simulation.
 *
 * - Parses and initializes the configuration
 *
 * - Allocates memory
 *
 * - Zero-initializes the log queue
 *
 * - Initializes the mutex
 *
 * @param sim Pointer to the simulation.
 * @param argc Number of program arguments.
 * @param argv Array containing the program arguments.
 *
 * @return Success of the initialization.
 */
bool	init_sim(t_sim *sim, const int argc, char *argv[])
{
	if (!init_config(sim, argc, argv))
		return (false);
	sim->philos = malloc(sizeof(t_philo) * sim->config.num_philos);
	sim->forks = malloc(sizeof(t_fork) * sim->config.num_philos);
	sim->queue = malloc(sizeof(t_queue));
	if (!sim->philos || !sim->forks || !sim->queue)
	{
		ft_perror("Malloc philos/forks/queue.");
		clean_sim(sim);
		return (false);
	}
	memset(sim->queue, 0, sizeof(t_queue));
	sim->queue->sim = sim;
	init_philos(sim);
	if (!init_mutex(sim))
	{
		ft_perror("Init mutex.");
		clean_sim(sim);
		return (false);
	}
	return (true);
}

/**
 * @brief Begins the simulation.
 *
 * - Creates the threads
 *
 * - Begins the monitoring routine
 *
 * - Joins all created threads
 *
 * @param sim Pointer to the simulation.
 */
void	simulate(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		if (\
pthread_create(&sim->philos[i].thread, NULL, philo_routine, &sim->philos[i]))
			break ;
		++i;
	}
	if (i == sim->config.num_philos)
	{
		if (pthread_create(&sim->thread_queue, NULL, logging, sim->queue))
			sim_error(sim);
		else
		{
			monitor_philos(sim);
			pthread_join(sim->thread_queue, NULL);
		}
	}
	else
		sim_error(sim);
	while (i--)
		pthread_join(sim->philos[i].thread, NULL);
}

/**
 * @brief Philosopher monitoring routine.
 *
 * Flags the simulation to be over if either:
 *
 * - All of the philosophers have eaten at least the optionally provided amount
 * of meals
 *
 * - A philosopher has died from starvation
 *
 * @note Performed periodically every 1 millisecond.
 *
 * @param sim Pointer to the simulation.
 */
static inline void	monitor_philos(t_sim *sim)
{
	int	i;

	while (!all_threads_running(sim))
		usleep(SPIN_TIME);
	while (sim->active)
	{
		usleep(SLEEP_TIME);
		pthread_mutex_lock(&sim->mutex_active);
		sim->active = (sim->philos_dined != sim->config.num_philos);
		i = sim->config.num_philos;
		while (i-- && sim->active)
		{
			if (\
time_now() - sim->philos[i].time_last_meal > sim->config.time_to_die)
			{
				log_state(&sim->philos[i], dead);
				sim->active = false;
			}
		}
		pthread_mutex_lock(&sim->queue->mutex);
		sim->queue->done = !sim->active;
		pthread_mutex_unlock(&sim->queue->mutex);
		pthread_mutex_unlock(&sim->mutex_active);
	}
}

/**
 * @brief Parses and initializes the configuration.
 *
 * @param sim Pointer to the simulation.
 * @param argc Number of program arguments.
 * @param argv Array containing the program arguments.
 *
 * @return Success of the parsing.
 */
static inline bool	init_config(t_sim *sim, const int argc, char *argv[])
{
	sim->config.num_meals = -1;
	if (\
!parse_int(argv[1], &sim->config.num_philos) || \
!parse_uint64(argv[2], &sim->config.time_to_die) || \
!parse_uint64(argv[3], &sim->config.time_to_eat) || \
!parse_uint64(argv[4], &sim->config.time_to_sleep) || \
(argc == 6 && !parse_int(argv[5], &sim->config.num_meals)))
		return (parse_error("Invalid input."));
	if (sim->config.num_philos < 1)
		return (parse_error("Invalid amount of philosophers."));
	if (\
sim->config.time_to_die < 1 || \
sim->config.time_to_eat < 1 || \
sim->config.time_to_sleep < 1)
		return (parse_error("Invalid amount of time."));
	if (argc == 6 && sim->config.num_meals < 1)
		return (parse_error("Invalid amount of meals."));
	sim->config.time_to_die *= 1000;
	sim->config.time_to_eat *= 1000;
	sim->config.time_to_sleep *= 1000;
	return (true);
}

/**
 * @brief Initializes all of the mutex.
 *
 * @note Counts and stores the number of fork mutex initialized for clean-up.
 *
 * @param sim Pointer to the simulation.
 *
 * @return Success of the initialization.
 */
static inline bool	init_mutex(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		if (!pthread_mutex_init(&sim->forks[i].mutex, NULL))
			++sim->num_fork_mutex_init;
		if (sim->num_fork_mutex_init == i)
			return (false);
		++i;
	}
	if (pthread_mutex_init(&sim->mutex_active, NULL))
		return (false);
	sim->mutex_active_init = true;
	if (pthread_mutex_init(&sim->queue->mutex, NULL))
		return (false);
	sim->mutex_queue_init = true;
	return (true);
}
