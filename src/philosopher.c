/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/20 04:46:33 by myli-pen         ###   ########.fr       */
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
static inline bool	take_forks(t_philo *p);

/**
 * @brief Philosopher thread routine.
 *
 * Rotates and logs philosopher's states while the simulation is active. Waits
 * until all of the threads are running.
 *
 * @param arg Pointer to the philosopher.
 *
 * @return Null pointer.
 */
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
		// if (p->meals != 0 && config.num_philos % 2 && time_now() <= p->time_last_meal + config.time_to_eat * 2)
		// {
		// 	uint64_t t = p->time_last_meal + config.time_to_eat * 2 - time_now();
		// 	//usleep(t);
		// 	//usleep(1000);
		// }
		log_state(p, thinking);
		eat(p);
		if (!is_active(p->sim))
			break ;
		log_state(p, sleeping);
		wait_until(config.time_to_sleep, p->sim);
	}
	return (NULL);
}

/**
 * @brief Waits until all of the threads are running.
 *
 * Further delays all even philosophers until odd ones are done waiting.
 *
 * @param p Pointer to the philosopher.
 */
static inline void	wait_until_start(t_philo *p)
{
	bool	delay;

	delay = true;
	pthread_mutex_lock(&p->sim->mutex_active);
	++p->sim->threads_running;
	pthread_mutex_unlock(&p->sim->mutex_active);
	while (!all_threads_running(p->sim))
		usleep(SPIN_TIME);
	if (p->id % 2 == 0)
	{
		while (delay)
		{
			pthread_mutex_lock(&p->sim->mutex_active);
			delay = (\
p->sim->threads_odd_init != \
p->sim->config.num_philos / 2 + (p->sim->config.num_philos % 2) && \
!p->sim->error);
			pthread_mutex_unlock(&p->sim->mutex_active);
			usleep(SPIN_TIME);
		}
	}
	if (p->id % 2)
	{
		pthread_mutex_lock(&p->sim->mutex_active);
		++p->sim->threads_odd_init;
		pthread_mutex_unlock(&p->sim->mutex_active);
	}
}

/**
 * @brief Checks for and handles the case where there is only one philosopher.
 *
 * @param p Pointer to the philosopher.
 *
 * @return Status whether there is only one philosopher.
 */
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

/**
 * @brief Eating action.
 *
 * - Reserves both forks needed for eating
 *
 * - Sets the timestamp for the meal
 *
 * - Informs the simulation when the optionally provided amount of meals was
 * eaten
 *
 * - Logs all the states
 *
 * @param p Pointer to the philosopher.
 */
static inline void	eat(t_philo *p)
{
	if (!take_forks(p))
		return ;
	pthread_mutex_lock(&p->sim->mutex_active);
	if (!p->sim->active)
	{
		pthread_mutex_unlock(&p->fork_r->mutex);
		pthread_mutex_unlock(&p->fork_l->mutex);
		pthread_mutex_unlock(&p->sim->mutex_active);
		return ;
	}
	p->meals = (p->meals + 1) % ((int64_t)INT_MAX + 1);
	p->time_last_meal = time_now();
	if (p->meals == p->sim->config.num_meals)
		++p->sim->philos_dined;
	pthread_mutex_unlock(&p->sim->mutex_active);
	log_state(p, eating);
	wait_until(p->sim->config.time_to_eat, p->sim);
	pthread_mutex_unlock(&p->fork_r->mutex);
	pthread_mutex_unlock(&p->fork_l->mutex);
}

/**
 * @brief Takes the forks with mutex.
 *
 * @param p Pointer to the philosopher.
 *
 * @return Success of the operation.
 */
static inline bool	take_forks(t_philo *p)
{
	pthread_mutex_lock(&p->fork_l->mutex);
	if (!is_active(p->sim))
	{
		pthread_mutex_unlock(&p->fork_l->mutex);
		return (false);
	}
	log_state(p, taking_a_fork);
	pthread_mutex_lock(&p->fork_r->mutex);
	if (!is_active(p->sim))
	{
		pthread_mutex_unlock(&p->fork_r->mutex);
		pthread_mutex_unlock(&p->fork_l->mutex);
		return (false);
	}
	log_state(p, taking_a_fork);
	return (true);
}
