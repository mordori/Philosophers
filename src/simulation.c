/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 21:45:59 by myli-pen         ###   ########.fr       */
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
		clean_sim(sim);
		return (false);
	}
	memset(sim->queue, 0, sizeof(t_queue));
	init_philos(sim);
	if (!init_mutex(sim))
	{
		ft_perror("Init mutex.");
		clean_sim(sim);
		return (false);
	}
	return (true);
}

void	simulate(t_sim *sim)
{
	int	i;

	sim->time_start = time_now();
	i = 0;
	while (i < sim->config.num_philos)
	{
		if (\
pthread_create(&sim->philos[i].thread, NULL, philo_routine, &sim->philos[i]))
			break ;
		++i;
	}
	if (i == sim->config.num_philos)
	{
		if (pthread_create(&sim->thread_queue, NULL, logging, sim->queue))
			sim_error(sim);
		else
		{
			monitor_philos(sim);
			pthread_join(sim->thread_queue, NULL);
		}
	}
	else
		sim_error(sim);
	while (i--)
		pthread_join(sim->philos[i].thread, NULL);
}

static inline void	monitor_philos(t_sim *sim)
{
	int	i;

	while (!threads_init(sim))
		usleep(SPIN_TIME);
	while (sim->active)
	{
		wait_ms(1);
		pthread_mutex_lock(&sim->mutex_active);
		sim->active = (sim->philos_dined != sim->config.num_philos);
		i = sim->config.num_philos;
		while (i--)
		{
			if (\
time_now() - sim->philos[i].time_last_meal > sim->config.time_to_die)
			{
				log_state(&sim->philos[i], dead);
				sim->active = false;
				break ;
			}
		}
		pthread_mutex_lock(&sim->queue->mutex);
		sim->queue->done = !sim->active;
		pthread_mutex_unlock(&sim->queue->mutex);
		pthread_mutex_unlock(&sim->mutex_active);
	}
}

static inline bool	init_config(t_sim *sim, const int argc, char *argv[])
{
	bool	result;

	result = true;
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
	{
		ft_perror("Invalid amount of philosophers.");
		result = false;
	}
	if (\
sim->config.time_to_die < 1 || \
sim->config.time_to_eat < 1 || \
sim->config.time_to_sleep < 1)
	{
		ft_perror("Invalid amount of time.");
		result = false;
	}
	if (argc == 6 && sim->config.num_meals < 1)
	{
		ft_perror("Invalid amount of meals.");
		result = false;
	}
	return (result);
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
			return (false);
		++i;
	}
	if (pthread_mutex_init(&sim->mutex_active, NULL))
		return (false);
	sim->mutex_active_init = true;
	if (pthread_mutex_init(&sim->queue->mutex, NULL))
		return (false);
	sim->mutex_queue_init = true;
	return (true);
}
