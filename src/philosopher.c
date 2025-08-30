/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 21:30:34 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline void	print_state(t_philo *philo, char *str);

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_config	config;

	philo = (t_philo *)arg;
	config = philo->sim->config;
	while (!get_with_mutex(philo->sim->start, &philo->sim->mutex_active))
		usleep(SPIN_TIME);
	philo->time_last_meal = time_now();
	while (get_with_mutex(philo->sim->active, &philo->sim->mutex_active))
	{
		print_state(philo, "is thinking");
		//take forks
		print_state(philo, "has taken a fork");
		//exit if dead
		print_state(philo, "is eating");
		usleep(config.time_to_eat);
		//release forks
		++philo->meals;
		if (philo->meals == config.num_meals)
			break ;
		philo->meals %= INT_MAX;
		print_state(philo, "is sleeping");
		usleep(config.time_to_sleep);
	}
	return (NULL);
}

static inline void	print_state(t_philo *philo, char *str)
{
	int64_t	time;

	time = time_now() - philo->sim->time_start;
	pthread_mutex_lock(&philo->sim->mutex_print);
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->sim->mutex_print);
}
