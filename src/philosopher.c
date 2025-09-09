/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/10 00:36:37 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline bool	eat(t_philo *philo);
static inline bool	print_state(t_philo *philo, char *str);
static inline bool	single_philo(t_philo *philo);
static inline int	take_forks(t_philo *philo);

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
	if (single_philo(philo))
		return (NULL);
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

static inline bool	single_philo(t_philo *philo)
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
	int	result;

	while (philo->sim->active)
	{
		result = take_forks(philo);
		if (result == SUCCESS)
			break ;
		else if (result == EXIT)
			return (false);
		usleep(SPIN_TIME);
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

static inline int	take_forks(t_philo *philo)
{
	if ((!philo->meals && philo->id != philo->sim->config.num_philos) || \
philo->time_last_meal < philo->philo_l->time_last_meal || \
philo->philo_l->meals == philo->sim->config.num_meals)
	{
		pthread_mutex_lock(&philo->fork_l->mutex);
		if (!print_state(philo, "has taken a fork"))
		{
			pthread_mutex_unlock(&philo->fork_l->mutex);
			return (EXIT);
		}
		while (philo->meals && \
philo->time_last_meal > philo->philo_r->time_last_meal && \
philo->philo_r->meals != philo->sim->config.num_meals)
{
	printf("asd %d\n", philo->id);
	usleep(SPIN_TIME);
}
		pthread_mutex_lock(&philo->fork_r->mutex);
		if (!print_state(philo, "has taken a fork"))
		{
			pthread_mutex_unlock(&philo->fork_l->mutex);
			pthread_mutex_unlock(&philo->fork_r->mutex);
			return (EXIT);
		}
		return (SUCCESS);
	}
	if (!philo->meals || \
philo->time_last_meal < philo->philo_r->time_last_meal || \
philo->philo_r->meals == philo->sim->config.num_meals)
	{
		pthread_mutex_lock(&philo->fork_r->mutex);
		if (!print_state(philo, "has taken a fork"))
		{
			pthread_mutex_unlock(&philo->fork_r->mutex);
			return (EXIT);
		}
		while ((philo->meals && \
philo->time_last_meal > philo->philo_l->time_last_meal && \
philo->philo_l->meals != philo->sim->config.num_meals) || \
(philo->id == philo->sim->config.num_philos && !philo->philo_l->meals))
{
	printf("asd %d\n", philo->id);
	usleep(SPIN_TIME);
}
		pthread_mutex_lock(&philo->fork_l->mutex);
		if (!print_state(philo, "has taken a fork"))
		{
			pthread_mutex_unlock(&philo->fork_l->mutex);
			pthread_mutex_unlock(&philo->fork_r->mutex);
			return (EXIT);
		}
		return (SUCCESS);
	}
	return (FAILURE);
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
