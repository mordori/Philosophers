/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/02 21:07:28 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"
#include "philosopher.h"

void	monitor_philo_death(t_sim *sim)
{
	int	i;

	wait_for(MIN_TASK_TIME + time_now(), sim);
	while (sim->philos_dined != sim->config.num_philos)
	{
		usleep(SPIN_TIME);
		i = sim->config.num_philos;
		while (i--)
		{
			pthread_mutex_lock(&sim->philos[i].mutex);
			if (sim->philos[i].meals == sim->config.num_meals)
			{
				pthread_mutex_unlock(&sim->philos[i].mutex);
				continue ;
			}
			pthread_mutex_unlock(&sim->philos[i].mutex);
			if (time_now() - sim->philos[i].time_last_meal > \
sim->config.time_to_die)
			{
				print_state(&sim->philos[i], "died");
				sim->active = false;
				return ;
			}
		}
	}
}

void	wait_for(int64_t target, t_sim *sim)
{
	int64_t	current;
	int64_t	interval;

	current = time_now();
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
	int64_t			result;

	gettimeofday(&tv, NULL);
	result = (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
	return (result);
}

void	clean_sim(t_sim *sim, pthread_mutex_t *print)
{
	int	i;

	if (print)
		pthread_mutex_destroy(print);
	i = 0;
	while (i < sim->num_philo_mutex_init)
	{
		pthread_mutex_destroy(&sim->philos[i].mutex);
		++i;
	}
	i = 0;
	while (i < sim->num_fork_mutex_init)
		pthread_mutex_destroy(&sim->forks[i++].mutex);
	free(sim->philos);
	free(sim->forks);
}

void	init_philos(t_sim *sim)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		philo = &sim->philos[i];
		philo->id = i + 1;
		philo->sim = sim;
		philo->meals = 0;
		philo->time_last_meal = time_now();
		philo->fork_l = &philo->sim->forks[philo->id - 1];
		philo->fork_r = &philo->sim->forks[philo->id % sim->config.num_philos];
		philo->fork_l->owner = NULL;
		philo->fork_l->reservation = NULL;
		++i;
	}
}
