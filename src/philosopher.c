/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/04 04:12:48 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"

static inline void	print_state(t_philo *philo, char *str);
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
	philo->time_last_meal = time_now();
	while (!philo->sim->start)
		usleep(SPIN_TIME);
	if (philo->id % 2 == 0)
		usleep(MIN_TASK_TIME / 2 * 1000);
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
	if (!philo->sim->active)
		return (false);
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
	t_philo	*philos;
	int		idx_l;
	int		idx_r;

	first_fork = philo->fork_l;
	second_fork = philo->fork_r;
	if (philo->fork_l > philo->fork_r)
	{
		first_fork = philo->fork_r;
		second_fork = philo->fork_l;
	}
	if (first_fork == second_fork)
	{
		pthread_mutex_unlock(&first_fork->mutex);
		return (false);
	}
	while (philo->sim->active)
	{
		philos = philo->sim->philos;
		idx_l = philo->id - 2;
		if (idx_l < 0)
			idx_l = philo->sim->config.num_philos - 1;
		idx_r = philo->id;
		if (philo->id == philo->sim->config.num_philos)
		{
			idx_r = idx_l;
			idx_l = 0;
		}
		if (philo->time_last_meal <= philos[idx_l].time_last_meal)
		{
			pthread_mutex_lock(&first_fork->mutex);
			print_state(philo, "has taken a fork");
			while (philo->time_last_meal > philos[idx_r].time_last_meal)
				usleep(SPIN_TIME);
			pthread_mutex_lock(&second_fork->mutex);
			print_state(philo, "has taken a fork");
			return (true);
		}
		if (philo->time_last_meal <= philos[idx_r].time_last_meal)
		{
			pthread_mutex_lock(&second_fork->mutex);
			print_state(philo, "has taken a fork");
			while (philo->time_last_meal > philos[idx_l].time_last_meal)
				usleep(SPIN_TIME);
			pthread_mutex_lock(&first_fork->mutex);
			print_state(philo, "has taken a fork");
			return (true);
		}
		usleep(SPIN_TIME);
	}
	return (false);
}

static inline bool	eat(t_philo *philo)
{
	if (!take_both_forks(philo))
		return (false);
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
