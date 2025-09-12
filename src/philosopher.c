/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/12 22:11:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline bool	eat(t_philo *philo);
static inline bool	is_single(t_philo *philo);
static inline void	take_forks(t_philo *philo);
static inline void	print_state(t_philo *philo, char *str);

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_config	config;

	philo = (t_philo *)arg;
	config = philo->sim->config;
	while (time_now() < philo->sim->time_start)
		usleep(SPIN_TIME);
	if (philo->id % 2 == 0)
		wait_ms(START_TIME, philo->sim);
	if (is_single(philo))
		return (NULL);
	while (is_active(philo->sim))
	{
		print_state(philo, "is thinking");
		while (philo->meals != 0 && time_now() < philo->time_last_meal + config.time_to_eat + 1)
			usleep(SPIN_TIME);
		eat(philo);
		if (philo->meals == philo->sim->config.num_meals)
			break ;
		print_state(philo, "is sleeping");
		wait_ms(config.time_to_sleep, philo->sim);
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
		wait_ms(philo->sim->config.time_to_die, philo->sim);
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (true);
	}
	return (false);
}

static inline bool	eat(t_philo *philo)
{
	take_forks(philo);
	if (is_active(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->mutex_active);
		philo->time_last_meal = time_now();
		philo->meals = (philo->meals + 1) % ((int64_t)INT_MAX + 1);
		if (philo->meals == philo->sim->config.num_meals)
			++philo->sim->philos_dined;
		pthread_mutex_unlock(&philo->sim->mutex_active);
		print_state(philo, "is eating");
		wait_ms(philo->sim->config.time_to_eat, philo->sim);
	}
	pthread_mutex_unlock(&philo->fork_r->mutex);
	pthread_mutex_unlock(&philo->fork_l->mutex);
	return (true);
}

static inline void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l->mutex);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&philo->fork_r->mutex);
	print_state(philo, "has taken a fork");
}

static inline void	print_state(t_philo *philo, char *str)
{
	int64_t	time;

	pthread_mutex_lock(&philo->sim->mutex_print);
	if (is_active(philo->sim))
	{
		time = time_now() - philo->sim->time_start;
		printf("%ld %d %s\n", time, philo->id, str);
	}
	pthread_mutex_unlock(&philo->sim->mutex_print);
}
