/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/03 03:10:53 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"
#include "errors.h"

static inline bool	eat(t_philo *philo);
static inline bool	take_both_forks(t_philo *philo);
static inline bool	take_fork(t_philo *philo, t_fork *fork);
static inline void	print_state(t_philo *philo, char *str);

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
		usleep(INITIAL_WAIT_TIME);
	philo->time_last_meal = time_now();
	while (philo->sim->active)
	{
		print_state(philo, "is thinking");
		if (!eat(philo) || philo->meals == philo->sim->config.num_meals)
			break ;
		print_state(philo, "is sleeping");
		wait_for(config.time_to_sleep, philo->sim);
	}
	return (NULL);
}

static inline bool	take_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&fork->mutex_reservation);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&fork->mutex_reservation);
		return (false);
	}
	pthread_mutex_lock(&fork->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&fork->mutex_reservation);
		pthread_mutex_unlock(&fork->mutex);
		return (false);
	}
	pthread_mutex_unlock(&fork->mutex_reservation);
	print_state(philo, "has taken a fork");
	return (true);
}

static inline bool	take_both_forks(t_philo *philo)
{
	if (!take_fork(philo, philo->fork_l))
		return (false);
	if (philo->fork_l == philo->fork_r)
	{
		pthread_mutex_unlock(&philo->fork_l->mutex);
		wait_for(philo->sim->config.time_to_die, philo->sim);
		return (false);
	}
	if (!take_fork(philo, philo->fork_r))
	{
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (false);
	}
	return (true);
}

static inline bool	eat(t_philo *philo)
{
	if (!take_both_forks(philo))
		return (false);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&philo->fork_l->mutex);
		pthread_mutex_unlock(&philo->fork_r->mutex);
		return (false);
	}
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

static inline void	print_state(t_philo *philo, char *str)
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
