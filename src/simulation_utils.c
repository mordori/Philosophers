/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/22 19:06:29 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"
#include "philosopher.h"
#include "logging.h"
#include "timing.h"

/**
 * @brief Performs activity check of the simulation.
 *
 * @param sim Pointer to the simulation.
 *
 * @return Simulation activity status.
 */
bool	is_active(t_sim *sim)
{
	bool	result;

	pthread_mutex_lock(&sim->mutex_active);
	result = (sim->active && !sim->error);
	pthread_mutex_unlock(&sim->mutex_active);
	return (result);
}

/**
 * @brief Deactivates the simulation if the queue is inactive.
 *
 * @param sim Pointer to the simulation.
 */
void	sync_status(t_sim *sim)
{
	pthread_mutex_lock(&sim->queue->mutex);
	if (sim->queue->done)
		sim->active = false;
	pthread_mutex_unlock(&sim->queue->mutex);
}

/**
 * @brief Checks if all of the created threads are running.
 *
 * @note Activates the simulation and initializes the timestamps for the
 * simulation start and last meals of the philosophers.
 *
 * @param sim Pointer to the simulation.
 *
 * @return Running status of the created threads. Also sets the status when an
 * error has occurred in intializing the threads or queue hits capacity so that
 * the existing ones will get terminated and joined.
 */
bool	all_threads_running(t_sim *sim)
{
	bool	result;
	int		i;

	pthread_mutex_lock(&sim->mutex_active);
	pthread_mutex_lock(&sim->queue->mutex);
	result = \
(sim->threads_running == sim->config.num_philos && sim->queue->init);
	if (sim->error || sim->queue->done)
		result = true;
	else if (result && !sim->active)
	{
		sim->time_start = time_now();
		i = 0;
		while (i < sim->config.num_philos)
		{
			sim->philos[i].time_last_meal = sim->time_start;
			++i;
		}
		sim->active = true;
	}
	pthread_mutex_unlock(&sim->queue->mutex);
	pthread_mutex_unlock(&sim->mutex_active);
	return (result);
}

/**
 * @brief Destroys all initialized mutex and frees allocated memory.
 *
 * @param sim Pointer to the simulation.
 */
void	clean_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_fork_mutex_init)
		pthread_mutex_destroy(&sim->forks[i++].mutex);
	if (sim->mutex_active_init)
		pthread_mutex_destroy(&sim->mutex_active);
	if (sim->mutex_queue_init)
		pthread_mutex_destroy(&sim->queue->mutex);
	free(sim->philos);
	free(sim->forks);
	free(sim->queue);
}

/**
 * @brief Initializes the philosophers.
 *
 * @note Fork order is reversed for odd and even philosophers to avoid
 * deadlocks.
 *
 * @param sim Pointer to the simulation.
 */
void	init_philos(t_sim *sim)
{
	t_philo		*p;
	t_config	*config;
	int			i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		p = &sim->philos[i];
		config = &sim->config;
		p->id = i + 1;
		p->sim = sim;
		p->meals = 0;
		p->fork_l = &sim->forks[i];
		if (config->num_philos == 1)
			return ;
		if (i % 2 == 0)
			p->fork_r = &sim->forks[(i + 1) % config->num_philos];
		else
		{
			p->fork_l = &sim->forks[(i + 1) % config->num_philos];
			p->fork_r = &sim->forks[i];
		}
		++i;
	}
}
