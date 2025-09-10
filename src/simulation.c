/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/10 12:23:47 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "errors.h"
#include "philosopher.h"
#include "parsing.h"
#include "simulation_utils.h"

static inline bool	init_config(t_sim *sim, const int argc, char *argv[]);
static inline bool	init_mutex(t_sim *sim);
static inline bool	validate_config(const t_sim *sim, const int argc);

bool	init_sim(t_sim *sim, const int argc, char *argv[])
{
	if (!init_config(sim, argc, argv))
		return (false);
	sim->philos = malloc(sizeof(t_philo) * sim->config.num_philos);
	sim->forks = malloc(sizeof(t_fork) * sim->config.num_philos);
	if (!sim->philos || !sim->forks)
	{
		ft_perror("Malloc philos/forks.");
		clean_sim(sim, NULL);
		return (false);
	}
	init_philos(sim);
	sim->num_philo_mutex_init = 0;
	sim->num_fork_mutex_init = 0;
	if (!init_mutex(sim))
	{
		ft_perror("Init mutex.");
		clean_sim(sim, NULL);
		return (false);
	}
	sim->philos_dined = 0;
	sim->threads = 0;
	sim->start = false;
	sim->active = false;
	return (true);
}

void	simulate(t_sim *sim)
{
	int	i;

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
	while (sim->threads < i)
		wait_for(SPIN_TIME, sim);
	if (sim->threads == sim->config.num_philos)
		sim->active = true;
	sim->time_start = time_now();
	sim->start = true;
	if (sim->active)
		monitor_philo_death(sim);
	while (i--)
	{
		if (pthread_join(sim->philos[i].thread, NULL))
			ft_perror("Failed to join a thread.");
	}
}

static inline bool	init_config(t_sim *sim, const int argc, char *argv[])
{
	sim->config.num_meals = -1;
	if (\
!parse_int(argv[1], &sim->config.num_philos) || \
!parse_int(argv[2], &sim->config.time_to_die) || \
!parse_int(argv[3], &sim->config.time_to_eat) || \
!parse_int(argv[4], &sim->config.time_to_sleep) || \
(argc == 6 && !parse_int(argv[5], &sim->config.num_meals)))
	{
		ft_perror("Invalid input.");
		return (false);
	}
	if (!validate_config(sim, argc))
		return (false);
	return (true);
}

static inline bool	validate_config(const t_sim *sim, const int argc)
{
	const t_config	config = sim->config;

	if (config.num_philos < 1 || config.num_philos > MAX_PHILOS)
		ft_perror("Invalid number of philosophers.");
	if (config.time_to_die < MIN_TASK_TIME)
		ft_perror("Invalid time to die.");
	if (config.time_to_eat < MIN_TASK_TIME)
		ft_perror("Invalid time to eat.");
	if (config.time_to_sleep < MIN_TASK_TIME)
		ft_perror("Invalid time to sleep.");
	if (argc == 6 && config.num_meals < 1)
		ft_perror("Invalid number of meals.");
	if (\
config.num_philos < 1 || config.num_philos > MAX_PHILOS || \
config.time_to_die < MIN_TASK_TIME || \
config.time_to_eat < MIN_TASK_TIME || \
config.time_to_sleep < MIN_TASK_TIME || \
(argc == 6 && config.num_meals < 1))
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
			return (false);
		++i;
	}
	if (pthread_mutex_init(&sim->mutex, NULL))
		return (false);
	return (true);
}
