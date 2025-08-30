/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 21:25:06 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "errors.h"
#include "philosopher.h"
#include "string_utils.h"
#include "simulation_utils.h"

static inline bool	init_config(t_sim *sim, const int argc, char *argv[]);
static inline bool	validate_config(const t_sim *sim, const int argc);
static inline void	monitor_philos(t_sim *sim);

bool	init_sim(t_sim *sim, const int argc, char *argv[])
{
	int	i;

	if (!init_config(sim, argc, argv))
		return (false);
	sim->philos = malloc(sizeof(t_philo) * sim->config.num_philos);
	sim->forks = malloc(sizeof(t_fork) * sim->config.num_philos);
	if (!sim->philos || !sim->forks)
	{
		free_sim(sim);
		ft_perror("Malloc philos or forks.");
		return (false);
	}
	i = -1;
	while (++i < sim->config.num_philos)
	{
		sim->forks[i].id = i + 1;
		sim->philos[i].id = i + 1;
		sim->philos[i].sim = sim;
		sim->philos[i].meals = 0;
		sim->philos[i].time_last_meal = time_now();
	}
	pthread_mutex_init(&sim->mutex_active, NULL);
	pthread_mutex_init(&sim->mutex_print, NULL);
	sim->start = false;
	sim->active = false;
	return (true);
}

bool	simulate(t_sim *sim)
{
	int		i;
	bool	result;

	result = true;
	i = -1;
	while (++i < sim->config.num_philos)
		if (pthread_create(&sim->philos[i].thread, NULL,
			philo_routine, &sim->philos[i]))
		{
			ft_perror("Failed to create a thread.");
			result = false;
			break ;
		}
	if (i == sim->config.num_philos)
	{
		set_with_mutex(&sim->active, true, &sim->mutex_active);
		monitor_philos(sim);
		sim->time_start = time_now();
	}
	set_with_mutex(&sim->start, true, &sim->mutex_active);
	while (i--)
		if(pthread_join(sim->philos[i].thread, NULL))
		{
			ft_perror("Failed to join a thread.");
			result = false;
		}
	return (result);
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
		return (false);
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
	{
		return (false);
	}
	return (true);
}

static inline void	monitor_philos(t_sim *sim)
{
	int		i;
	int64_t	time;

	while (get_with_mutex(sim->active, &sim->mutex_active))
	{
		i = sim->config.num_philos;
		while (i--)
		{
			time = time_now() - sim->philos[i].time_last_meal;
			if (time > sim->config.time_to_die)
			{
				pthread_mutex_lock(&sim->mutex_print);
				printf("%ld %d died\n", time, sim->philos[i].id);
				pthread_mutex_unlock(&sim->mutex_print);
				set_with_mutex(&sim->active, false, &sim->mutex_active);
				break ;
			}
		}
		usleep(SPIN_TIME);
	}
}
