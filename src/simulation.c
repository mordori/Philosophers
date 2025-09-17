/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 20:22:10 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "errors.h"
#include "philosopher.h"
#include "parsing.h"
#include "simulation_utils.h"
#include "logging.h"
#include "timing.h"

static inline void	monitor_philos(t_sim *sim);
static inline bool	init_config(t_sim *sim, const int argc, char *argv[]);
static inline bool	init_mutex(t_sim *sim);

bool	init_sim(t_sim *sim, const int argc, char *argv[])
{
	if (!init_config(sim, argc, argv))
		return (false);
	sim->philos = malloc(sizeof(t_philo) * sim->config.num_philos);
	sim->forks = malloc(sizeof(t_fork) * sim->config.num_philos);
	sim->queue = malloc(sizeof(t_queue));
	if (!sim->philos || !sim->forks || !sim->queue)
	{
		ft_perror("Malloc philos/forks/queue.");
		clean_sim(sim, NULL, NULL, NULL);
		return (false);
	}
	memset(sim->queue, 0, sizeof(t_queue));
	init_philos(sim);
	if (!init_mutex(sim))
	{
		ft_perror("Init mutex.");
		return (false);
	}
	return (true);
}

void	simulate(t_sim *sim)
{
	int	i;

	sim->time_start = time_now() + START_TIME;
	sim->active = true;
	i = 0;
	while (i < sim->config.num_philos)
	{
		if (pthread_create(&sim->philos[i].thread, NULL, \
philo_routine, &sim->philos[i]))
		{
			ft_perror("Failed to create a thread.");
			break ;
		}
		++i;
	}
	if (i == sim->config.num_philos)
	{
		if (pthread_create(&sim->thread_queue, NULL, logging, sim->queue))
			ft_perror("Failed to create a thread.");
		else
		{
			monitor_philos(sim);
			if (pthread_join(sim->thread_queue, NULL))
				ft_perror("Failed to join a thread.");
		}
	}
	while (i--)
	{
		if (pthread_join(sim->philos[i].thread, NULL))
			ft_perror("Failed to join a thread.");
	}
}

static inline void	monitor_philos(t_sim *sim)
{
	int			i;
	uint64_t	last_meal;

	while (time_now() < sim->time_start)
		wait_ms(START_TIME);
	while (is_active(sim))
	{
		i = sim->config.num_philos;
		while (i--)
		{
			pthread_mutex_lock(&sim->mutex_active);
			last_meal = sim->philos[i].time_last_meal;
			pthread_mutex_unlock(&sim->mutex_active);
			if (time_now() - last_meal > sim->config.time_to_die)
			{
				log_state(&sim->philos[i], dead);
				flush_queue(sim->queue);
				pthread_mutex_lock(&sim->mutex_active);
				sim->active = false;
				pthread_mutex_unlock(&sim->mutex_active);
				break ;
			}
		}
		pthread_mutex_lock(&sim->mutex_active);
		pthread_mutex_lock(&sim->queue->mutex);
		if (sim->philos_dined == sim->config.num_philos)
			sim->active = false;
		if (!sim->active)
			sim->queue->done = true;
		pthread_mutex_unlock(&sim->queue->mutex);
		pthread_mutex_unlock(&sim->mutex_active);
		wait_ms(MONITOR_TIME);
	}
}

static inline bool	init_config(t_sim *sim, const int argc, char *argv[])
{
	sim->config.num_meals = -1;
	if (\
!parse_int(argv[1], &sim->config.num_philos) || \
!parse_uint64(argv[2], &sim->config.time_to_die) || \
!parse_uint64(argv[3], &sim->config.time_to_eat) || \
!parse_uint64(argv[4], &sim->config.time_to_sleep) || \
(argc == 6 && !parse_int(argv[5], &sim->config.num_meals)))
	{
		ft_perror("Invalid input.");
		return (false);
	}
	if (sim->config.num_philos < 1)
		ft_perror("Invalid amount of philosophers.");
	if (argc == 6 && sim->config.num_meals < 1)
		ft_perror("Invalid amount of meals.");
	if (sim->config.num_philos < 1 || (argc == 6 && sim->config.num_meals < 1))
		return (false);
	return (true);
}

static inline bool	init_mutex(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_philos)
	{
		if (!pthread_mutex_init(&sim->forks[i].mutex, NULL))
			++sim->num_fork_mutex_init;
		if (sim->num_fork_mutex_init == i)
		{
			clean_sim(sim, NULL, NULL, NULL);
			return (false);
		}
		++i;
	}
	if (pthread_mutex_init(&sim->mutex_print, NULL))
	{
		clean_sim(sim, NULL, NULL, NULL);
		return (false);
	}
	if (pthread_mutex_init(&sim->mutex_active, NULL))
	{
		clean_sim(sim, &sim->mutex_print, NULL, NULL);
		return (false);
	}
	if (pthread_mutex_init(&sim->queue->mutex, NULL))
	{
		clean_sim(sim, &sim->mutex_print, &sim->mutex_active, NULL);
		return (false);
	}
	return (true);
}
