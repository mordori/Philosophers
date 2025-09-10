/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/10 19:00:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"
#include "philosopher.h"

void	monitor_philo_death(t_sim *sim)
{
	int	i;

	wait_for(MIN_TASK_TIME, sim);
	while (sim->philos_dined != sim->config.num_philos)
	{
		usleep(SPIN_TIME);
		i = sim->config.num_philos;
		while (i--)
		{
			if (sim->philos[i].meals == sim->config.num_meals)
				continue ;
			if (time_now() - sim->philos[i].time_last_meal > \
sim->config.time_to_die)
			{
				sim->active = false;
				pthread_mutex_lock(&sim->mutex);
				printf("%ld %d %s\n", \
time_now() - sim->time_start, sim->philos[i].id, "died");
				pthread_mutex_unlock(&sim->mutex);
				return ;
			}
		}
	}
}

void	wait_for(int64_t duration, t_sim *sim)
{
	int64_t	current;
	int64_t	interval;
	int64_t	target;

	current = time_now();
	target = current + duration;
	while (current < target)
	{
		if (!sim->active)
			return ;
		interval = (target - current) * 1000;
		if (interval <= 0)
			return ;
		if (interval > SPIN_TIME)
			interval = SPIN_TIME;
		usleep(interval);
		current = time_now();
	}
}

int64_t	time_now(void)
{
	struct timeval	tv;
	int64_t			time;

	gettimeofday(&tv, NULL);
	time = (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
	return (time);
}

void	clean_sim(t_sim *sim, pthread_mutex_t *sim_mutex)
{
	int	i;

	if (sim_mutex)
		pthread_mutex_destroy(sim_mutex);
	i = 0;
	while (i < sim->num_fork_mutex_init)
		pthread_mutex_destroy(&sim->forks[i++].mutex);
	free(sim->philos);
	free(sim->forks);
}

void	init_philos(t_sim *sim)
{
	static int	i = -1;

	while (++i < sim->config.num_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].sim = sim;
		sim->philos[i].meals = 0;
		sim->philos[i].time_last_meal = time_now();
		sim->philos[i].fork_l = &sim->forks[i];
		if (sim->config.num_philos == 1)
			return ;
		sim->philos[i].fork_r = &sim->forks[(i + 1) % sim->config.num_philos];
		if (i == sim->config.num_philos - 1)
		{
			sim->philos[i].fork_l = &sim->forks[0];
			sim->philos[i].fork_r = &sim->forks[i];
		}
	}
}
