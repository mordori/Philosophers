/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/02 21:09:39 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline bool	eat(t_philo *philo);
static inline bool	take_both_forks(t_philo *philo);
static inline bool	take_fork(t_philo *philo, t_fork *fork);

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_config	config;

	philo = (t_philo *)arg;
	config = philo->sim->config;
	++philo->sim->threads;
	while (!philo->sim->start)
		usleep(SPIN_TIME);
	if (philo->id % 2 == 0)
		usleep(SPIN_TIME);
	philo->time_last_meal = time_now();
	while (philo->sim->active)
	{
		print_state(philo, "is thinking");
		if (!eat(philo) || philo->meals == philo->sim->config.num_meals)
			break ;
		print_state(philo, "is sleeping");
		wait_for(config.time_to_sleep + time_now(), philo->sim);
	}
	return (NULL);
}

void	print_state(t_philo *philo, char *str)
{
	int64_t	time;

	pthread_mutex_lock(&philo->sim->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&philo->sim->mutex);
		return ;
	}
	time = time_now() - philo->sim->time_start;
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->sim->mutex);
}

static inline bool	take_fork(t_philo *philo, t_fork *fork)
{
	while (fork->reservation)
		usleep(SPIN_TIME);
	while (fork->owner)
		usleep(SPIN_TIME);
	pthread_mutex_lock(&fork->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&fork->mutex);
		return (false);
	}
	fork->owner = philo;
	pthread_mutex_unlock(&fork->mutex);
	print_state(philo, "has taken a fork");
	return (true);
}

static inline bool	take_both_forks(t_philo *philo)
{
	if (!take_fork(philo, philo->fork_l))
		return (false);
	if (philo->fork_l == philo->fork_r)
	{
		wait_for(philo->sim->config.time_to_die + time_now(), philo->sim);
		return (false);
	}
	if (!take_fork(philo, philo->fork_r))
		return (false);
	return (true);
}

static inline bool	eat(t_philo *philo)
{
	if (!take_both_forks(philo))
		return (false);
	pthread_mutex_lock(&philo->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&philo->mutex);
		return (false);
	}
	philo->time_last_meal = time_now();
	philo->meals = (philo->meals + 1) % ((int64_t)INT_MAX + 1);
	if (philo->meals == philo->sim->config.num_meals)
		++philo->sim->philos_dined;
	print_state(philo, "is eating");
	pthread_mutex_unlock(&philo->mutex);
	wait_for(philo->sim->config.time_to_eat + time_now(), philo->sim);
	pthread_mutex_lock(&philo->fork_l->mutex);
	philo->fork_l->owner = NULL;
	pthread_mutex_lock(&philo->fork_l->mutex);
	pthread_mutex_unlock(&philo->fork_r->mutex);
	philo->fork_r->owner = NULL;
	pthread_mutex_unlock(&philo->fork_r->mutex);
	return (true);
}
