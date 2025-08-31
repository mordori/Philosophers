/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/31 03:19:32 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"
#include "philosopher.h"

void	monitor_philos(t_sim *sim)
{
	int		i;
	int64_t	time;

	while (true)
	{
		i = sim->config.num_philos;
		while (i--)
		{
			pthread_mutex_lock(&sim->philos[i].mutex);
			time = time_now() - sim->philos[i].time_last_meal;
			pthread_mutex_unlock(&sim->philos[i].mutex);
			if (time > sim->config.time_to_die)
			{
				pthread_mutex_lock(&sim->mutex_print);
				time = time_now() - sim->time_start;
				printf("%ld %d died\n", time, sim->philos[i].id);
				pthread_mutex_unlock(&sim->mutex_print);
				set_with_mutex(&sim->active, false, &sim->mutex_active);
				return ;
			}
		}
		usleep(SPIN_TIME);
	}
}

void	set_with_mutex(int *var, int val, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	*var = val;
	pthread_mutex_unlock(mutex);
}

int	get_with_mutex(const int var, pthread_mutex_t *mutex)
{
	int	result;

	pthread_mutex_lock(mutex);
	result = var;
	pthread_mutex_unlock(mutex);
	return (result);
}

int64_t	time_now(void)
{
	struct timeval	tv;
	int64_t			result;

	gettimeofday(&tv, NULL);
	result = (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
	return (result);
}

void	clean_sim(t_sim *sim, pthread_mutex_t *active, pthread_mutex_t *print)
{
	int	i;

	free(sim->philos);
	free(sim->forks);
	if (active)
		pthread_mutex_destroy(active);
	if (print)
		pthread_mutex_destroy(print);
	i = 0;
	while (i < sim->philos_init)
	{
		pthread_mutex_destroy(&sim->philos[i].mutex);
		++i;
	}
	i = 0;
	while (i < sim->forks_init)
	{
		pthread_mutex_destroy(&sim->forks[i].mutex);
		++i;
	}

}
