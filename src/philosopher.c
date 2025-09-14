/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/14 04:05:23 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline void	eat(t_philo *philo);
static inline bool	is_single(t_philo *philo);
static inline void	print_state(t_philo *philo, char *str);

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_config	config;

	philo = (t_philo *)arg;
	config = philo->sim->config;
	while (time_now() < philo->sim->time_start)
		wait_ms(START_TIME);
	if (is_single(philo))
		return (NULL);
	if (philo->id % 2 == 0)
		wait_ms(START_TIME);
	while (is_active(philo->sim))
	{
		print_state(philo, "is thinking");
		if (philo->meals != 0)
			wait_ms(1);
		eat(philo);
		print_state(philo, "is sleeping");
		wait_until(config.time_to_sleep, philo->sim);
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
		usleep(philo->sim->config.time_to_die * 1000);
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (true);
	}
	return (false);
}

static inline void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l->mutex);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&philo->fork_r->mutex);
	print_state(philo, "has taken a fork");
	if (is_active(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->mutex_active);
		philo->time_last_meal = time_now();
		philo->meals = (philo->meals + 1) % ((int64_t)INT_MAX + 1);
		if (philo->meals == philo->sim->config.num_meals)
			++philo->sim->philos_dined;
		pthread_mutex_unlock(&philo->sim->mutex_active);
		print_state(philo, "is eating");
		wait_until(philo->sim->config.time_to_eat, philo->sim);
	}
	pthread_mutex_unlock(&philo->fork_r->mutex);
	pthread_mutex_unlock(&philo->fork_l->mutex);
}

static inline void	print_state(t_philo *philo, char *str)
{
	uint64_t	time;

	pthread_mutex_lock(&philo->sim->mutex_print);
	if (is_active(philo->sim))
	{
		time = time_now() - philo->sim->time_start;
		printf("%lu %d %s\n", time, philo->id, str);
	}
	pthread_mutex_unlock(&philo->sim->mutex_print);
}
