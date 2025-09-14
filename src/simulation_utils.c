/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/14 02:41:14 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"
#include "philosopher.h"

bool	is_active(t_sim *sim)
{
	bool	result;

	pthread_mutex_lock(&sim->mutex_active);
	result = sim->active;
	pthread_mutex_unlock(&sim->mutex_active);
	return (result);
}

void	wait_until(uint64_t duration, t_sim *sim)
{
	uint64_t	time_end;

	time_end = time_now() + duration;
	while (time_now() < time_end && is_active(sim))
	{
		if (time_end - time_now() > 6)
			usleep(5000);
		else
			usleep(200);
	}
}

void	wait_ms(uint64_t duration)
{
	uint64_t	time_end;

	time_end = time_now() + duration;
	while (time_now() < time_end)
		usleep(200);
}

uint64_t	time_now(void)
{
	struct timeval	tv;
	uint64_t		time;

	gettimeofday(&tv, NULL);
	time = (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
	return (time);
}

void	clean_sim(t_sim *sim, pthread_mutex_t *mutex_print, \
pthread_mutex_t *mutex_active)
{
	int	i;

	i = 0;
	while (i < sim->num_fork_mutex_init)
		pthread_mutex_destroy(&sim->forks[i++].mutex);
	if (mutex_print)
		pthread_mutex_destroy(mutex_print);
	if (mutex_active)
		pthread_mutex_destroy(mutex_active);
	free(sim->philos);
	free(sim->forks);
}

void	init_philos(t_sim *sim)
{
	static int	i = 0;
	t_config	config;

	config = sim->config;
	while (i < sim->config.num_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].sim = sim;
		sim->philos[i].meals = 0;
		sim->philos[i].time_last_meal = time_now() + START_TIME;
		sim->philos[i].fork_l = &sim->forks[i];
		if (config.num_philos == 1)
			return ;
		if (i % 2 == 0)
		{
			sim->philos[i].fork_l = &sim->forks[i];
			sim->philos[i].fork_r = &sim->forks[(i + 1) % config.num_philos];
		}
		else
		{
			sim->philos[i].fork_l = &sim->forks[(i + 1) % config.num_philos];
			sim->philos[i].fork_r = &sim->forks[i];
		}
		++i;
	}
}
