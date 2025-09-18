/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 02:54:47 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"
#include "philosopher.h"
#include "logging.h"
#include "timing.h"

static inline bool	init_philo(t_sim *sim, int i);

bool	is_active(t_sim *sim)
{
	bool	result;

	pthread_mutex_lock(&sim->mutex_active);
	result = sim->active;
	pthread_mutex_unlock(&sim->mutex_active);
	return (result);
}

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

void	init_philos(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		if (!init_philo(sim, i))
			return ;
		++i;
	}
}

static inline bool	init_philo(t_sim *sim, int i)
{
	t_philo		*p;
	t_config	*config;

	p = &sim->philos[i];
	config = &sim->config;
	p->id = i + 1;
	p->sim = sim;
	p->meals = 0;
	p->time_last_meal = time_now() + START_TIME;
	p->fork_l = &sim->forks[i];
	if (config->num_philos == 1)
		return (false);
	if (i % 2 == 0)
	{
		p->fork_l = &sim->forks[i];
		p->fork_r = &sim->forks[(i + 1) % config->num_philos];
	}
	else
	{
		p->fork_l = &sim->forks[(i + 1) % config->num_philos];
		p->fork_r = &sim->forks[i];
	}
	return (true);
}
