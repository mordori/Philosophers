/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/31 03:29:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline bool	eat(t_philo *philo);
static inline void	print_state(t_philo *philo, char *str);

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_config	config;

	philo = (t_philo *)arg;
	config = philo->sim->config;
	while (!get_with_mutex(philo->sim->start, &philo->sim->mutex_active))
		usleep(SPIN_TIME);
	philo->fork_l = &philo->sim->forks[philo->id - 1];
	philo->fork_r = &philo->sim->forks[philo->id % philo->sim->config.num_philos];
	philo->time_last_meal = time_now();
	while (get_with_mutex(philo->sim->active, &philo->sim->mutex_active))
	{
		print_state(philo, "is thinking");
		if (!eat(philo))
			break;
		++philo->meals;
		if (philo->meals == config.num_meals)
			break ;
		philo->meals %= INT_MAX;
		print_state(philo, "is sleeping");
		usleep(config.time_to_sleep * 1000);
	}
	return (NULL);
}

static inline bool	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l->mutex);
	print_state(philo, "has taken a fork");
	if (philo->fork_l == philo->fork_r)
	{
		usleep(philo->sim->config.time_to_die * 1000);
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (false);
	}
	pthread_mutex_lock(&philo->fork_r->mutex);
	print_state(philo, "has taken a fork");
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->mutex);
	philo->time_last_meal = time_now();
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->sim->config.time_to_eat * 1000);
	pthread_mutex_unlock(&philo->fork_l->mutex);
	pthread_mutex_unlock(&philo->fork_r->mutex);
	return (true);
}


static inline void	print_state(t_philo *philo, char *str)
{
	int64_t	time;

	if (!get_with_mutex(philo->sim->active, &philo->sim->mutex_active))
		return ;
	pthread_mutex_lock(&philo->sim->mutex_print);
	time = time_now() - philo->sim->time_start;
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->sim->mutex_print);
}
