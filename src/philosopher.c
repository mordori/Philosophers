/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/10 16:57:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline bool	eat(t_philo *philo);
static inline bool	print_state(t_philo *philo, char *str);
static inline bool	is_single(t_philo *philo);
static inline bool	take_forks(t_philo *philo);

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_config	config;

	philo = (t_philo *)arg;
	config = philo->sim->config;
	++philo->sim->threads;
	philo->time_last_meal = time_now();
	while (!philo->sim->start)
		usleep(SPIN_TIME);
	if (philo->id % 2 == 0)
		wait_for(MIN_TASK_TIME / 2, philo->sim);
	if (is_single(philo))
		return (NULL);
	while (philo->sim->active)
	{
		print_state(philo, "is thinking");
		if (!eat(philo) || philo->meals == config.num_meals)
			break ;
		print_state(philo, "is sleeping");
		wait_for(config.time_to_sleep, philo->sim);
	}
	return (NULL);
}

static inline bool	is_single(t_philo *philo)
{
	if (philo->sim->config.num_philos == 1)
	{
		print_state(philo, "is thinking");
		pthread_mutex_lock(&philo->fork_l->mutex);
		print_state(philo, "has taken a fork");
		wait_for(philo->sim->config.time_to_die, philo->sim);
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (true);
	}
	return (false);
}

static inline bool	eat(t_philo *philo)
{

		take_forks(philo);

	philo->time_last_meal = time_now();
	philo->meals = (philo->meals + 1) % ((int64_t)INT_MAX + 1);
	if (philo->meals == philo->sim->config.num_meals)
		++philo->sim->philos_dined;
	print_state(philo, "is eating");
	wait_for(philo->sim->config.time_to_eat, philo->sim);
	pthread_mutex_unlock(&philo->fork_l->mutex);
	pthread_mutex_unlock(&philo->fork_r->mutex);
	return (true);
}

static inline bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l->mutex);
	if (!print_state(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (false);
	}
	pthread_mutex_lock(&philo->fork_r->mutex);
	if (!print_state(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(&philo->fork_l->mutex);
		pthread_mutex_unlock(&philo->fork_r->mutex);
		return (false);
	}
	return (true);
}

static inline bool	print_state(t_philo *philo, char *str)
{
	int64_t	time;

	pthread_mutex_lock(&philo->sim->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&philo->sim->mutex);
		return (false);
	}
	time = time_now() - philo->sim->time_start;
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->sim->mutex);
	return (true);
}
