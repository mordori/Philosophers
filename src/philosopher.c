/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 03:59:20 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"
#include "logging.h"
#include "timing.h"
#include "errors.h"

static inline void	eat(t_philo *philo);
static inline bool	is_single(t_philo *philo);

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
		log_state(philo, "is thinking");
		if (philo->meals != 0)
			wait_ms(1);
		eat(philo);
		log_state(philo, "is sleeping");
		wait_until(config.time_to_sleep, philo->sim);
	}
	return (NULL);
}

void	log_state(t_philo *philo, const char *state)
{
	uint64_t	ts;
	int			next;
	t_queue		*q;
	int			i;

	if (!is_active(philo->sim))
		return ;
	q = philo->sim->queue;
	ts = time_now() - philo->sim->time_start;
	pthread_mutex_lock(&q->mutex);
	next = (q->head + 1) % LOG_QUEUE_SIZE;
	if (next != q->tail)
	{
		q->buffer[q->head].timestamp = ts;
		q->buffer[q->head].philo_id = philo->id;
		i = 0;
		while (i < STATE_LENGTH - 1)
		{
			q->buffer[q->head].state[i] = state[i];
			++i;
		}
		q->buffer[q->head].state[i] = '\0';
		q->head = next;
	}
	else
		ft_perror("Log queue full.");
	pthread_mutex_unlock(&q->mutex);
}

static inline bool	is_single(t_philo *philo)
{
	if (philo->sim->config.num_philos == 1)
	{
		log_state(philo, "is thinking");
		pthread_mutex_lock(&philo->fork_l->mutex);
		log_state(philo, "has taken a fork");
		usleep(philo->sim->config.time_to_die * 1000);
		pthread_mutex_unlock(&philo->fork_l->mutex);
		return (true);
	}
	return (false);
}

static inline void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l->mutex);
	log_state(philo, "has taken a fork");
	pthread_mutex_lock(&philo->fork_r->mutex);
	log_state(philo, "has taken a fork");
	if (is_active(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->mutex_active);
		philo->time_last_meal = time_now();
		philo->meals = (philo->meals + 1) % ((int64_t)INT_MAX + 1);
		if (philo->meals == philo->sim->config.num_meals)
			++philo->sim->philos_dined;
		pthread_mutex_unlock(&philo->sim->mutex_active);
		log_state(philo, "is eating");
		wait_until(philo->sim->config.time_to_eat, philo->sim);
	}
	pthread_mutex_unlock(&philo->fork_r->mutex);
	pthread_mutex_unlock(&philo->fork_l->mutex);
}
