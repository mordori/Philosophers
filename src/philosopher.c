/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/01 04:25:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

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
	philo->fork_l = &philo->sim->forks[philo->id - 1];
	philo->fork_r = &philo->sim->forks[philo->id % config.num_philos];
	++philo->sim->philos_init;
	while (!philo->sim->start)
		usleep(SPIN_TIME);
	wait_for(philo->sim->time_start + 100, philo->sim);
	philo->sim->time_start = time_now();
	philo->time_last_meal = philo->sim->time_start;
	while (philo->sim->active)
	{
		print_state(philo, "is thinking");
		if (!eat(philo))
			break;
		if (philo->meals == config.num_meals)
		{
			++philo->sim->philos_dined;
			break ;
		}
		philo->meals %= INT_MAX;
		print_state(philo, "is sleeping");
		wait_for(config.time_to_sleep + time_now(), philo->sim);
	}
	return (NULL);
}

static inline bool	take_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&fork->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&fork->mutex);
		return (false);
	}
	print_state(philo, "has taken a fork");
	return (true);
}

static inline bool	take_both_forks(t_philo *philo)
{
	t_fork	*first_fork;
	t_fork	*second_fork;

	first_fork = philo->fork_l;
	second_fork = philo->fork_r;
	if (philo->fork_l > philo->fork_r)
	{
		first_fork = philo->fork_r;
		second_fork = philo->fork_l;
	}
	if (!take_fork(philo, first_fork))
		return (false);
	if (first_fork == second_fork)
	{
		wait_for(philo->sim->config.time_to_die + time_now(), philo->sim);
		pthread_mutex_unlock(&first_fork->mutex);
		return (false);
	}
	if (!take_fork(philo, second_fork))
	{
		pthread_mutex_unlock(&first_fork->mutex);
		return (false);
	}
	return (true);
}

static inline bool	eat(t_philo *philo)
{
	if (philo->id % 2)
		usleep(1000);
	if (!take_both_forks(philo))
		return (false);
	pthread_mutex_lock(&philo->mutex);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&philo->fork_l->mutex);
		pthread_mutex_unlock(&philo->fork_r->mutex);
		pthread_mutex_unlock(&philo->mutex);
		return (false);
	}
	philo->time_last_meal = time_now();
	++philo->meals;
	print_state(philo, "is eating");
	pthread_mutex_unlock(&philo->mutex);
	wait_for(philo->sim->config.time_to_eat + time_now(), philo->sim);
	pthread_mutex_unlock(&philo->fork_l->mutex);
	pthread_mutex_unlock(&philo->fork_r->mutex);
	return (true);
}

static inline void	print_state(t_philo *philo, char *str)
{
	int64_t	time;

	pthread_mutex_lock(&philo->sim->mutex_print);
	if (!philo->sim->active)
	{
		pthread_mutex_unlock(&philo->sim->mutex_print);
		return ;
	}
	time = time_now() - philo->sim->time_start;
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->sim->mutex_print);
}
