/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 22:14:17 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"
#include "logging.h"
#include "timing.h"
#include "errors.h"

static inline void	wait_until_start(t_philo *p);
static inline void	eat(t_philo *p);
static inline bool	is_single(t_philo *p);

void	*philo_routine(void *arg)
{
	t_philo		*p;
	t_config	config;

	p = (t_philo *)arg;
	config = p->sim->config;
	wait_until_start(p);
	if (is_single(p))
		return (NULL);
	while (is_active(p->sim))
	{
		log_state(p, thinking);
		if (p->meals != 0)
			wait_ms(1);
		eat(p);
		log_state(p, sleeping);
		wait_until(config.time_to_sleep, p->sim);
	}
	return (NULL);
}

static inline void	wait_until_start(t_philo *p)
{
	bool	delay;

	pthread_mutex_lock(&p->sim->mutex_active);
	++p->sim->threads;
	pthread_mutex_unlock(&p->sim->mutex_active);
	while (!threads_init(p->sim))
		usleep(SPIN_TIME);
	if (p->id % 2 == 0)
	{
		while (true)
		{
			usleep(SPIN_TIME);
			pthread_mutex_lock(&p->sim->mutex_active);
			delay = (p->sim->threads_init != p->sim->config.num_philos / 2 + \
(p->sim->config.num_philos % 2));
			pthread_mutex_unlock(&p->sim->mutex_active);
			if (!delay)
				break ;
		}
	}
	pthread_mutex_lock(&p->sim->mutex_active);
	if (p->id % 2)
		++p->sim->threads_init;
	pthread_mutex_unlock(&p->sim->mutex_active);
}

static inline bool	is_single(t_philo *p)
{
	if (p->sim->config.num_philos == 1)
	{
		log_state(p, thinking);
		pthread_mutex_lock(&p->fork_l->mutex);
		log_state(p, taking_a_fork);
		usleep(p->sim->config.time_to_die * 1000);
		pthread_mutex_unlock(&p->fork_l->mutex);
		return (true);
	}
	return (false);
}

static inline void	eat(t_philo *p)
{
	pthread_mutex_lock(&p->fork_l->mutex);
	log_state(p, taking_a_fork);
	pthread_mutex_lock(&p->fork_r->mutex);
	log_state(p, taking_a_fork);
	if (is_active(p->sim))
	{
		pthread_mutex_lock(&p->sim->mutex_active);
		p->time_last_meal = time_now();
		p->meals = (p->meals + 1) % ((int64_t)INT_MAX + 1);
		if (p->meals == p->sim->config.num_meals)
			++p->sim->philos_dined;
		pthread_mutex_unlock(&p->sim->mutex_active);
		log_state(p, eating);
		wait_until(p->sim->config.time_to_eat, p->sim);
	}
	pthread_mutex_unlock(&p->fork_r->mutex);
	pthread_mutex_unlock(&p->fork_l->mutex);
}
