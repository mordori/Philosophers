/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 05:44:48 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

static inline bool	init_value(char *str, int *value);
static inline bool	validate_sim_args(const t_sim *sim, const int argc);

bool	simulate(t_sim *sim)
{
	int		i;
	bool	result;

	result = true;
	i = -1;
	while (++i < sim->num_philos)
		if (pthread_create(&sim->philos[i].thread, NULL,
			philo_routine, &sim->philos[i]))
		{
			ft_error("Failed to create a thread.");
			set_with_mutex(&sim->active, false, &sim->lock);
			result = false;
			break ;
		}
	if (i == sim->num_philos)
		set_with_mutex(&sim->active, true, &sim->lock);
	while (i--)
		if(pthread_join(sim->philos[i].thread, NULL))
		{
			ft_error("Failed to join a thread.");
			result = false;
		}
	return (result);
}

bool	init_sim(t_sim *sim, const int argc, char *argv[])
{
	sim->num_meals = -1;
	if (\
!init_value(argv[1], &sim->num_philos) || \
!init_value(argv[2], &sim->time_to_die) || \
!init_value(argv[3], &sim->time_to_eat) || \
!init_value(argv[4], &sim->time_to_sleep) || \
(argc == 6 && !init_value(argv[5], &sim->num_meals)))
		return (false);
	if (!validate_sim_args(sim, argc))
		return (false);
	sim->philos = malloc(sizeof(t_philo) * sim->num_philos);
	if (!sim->philos)
	{
		ft_error("Malloc philos.");
		return (false);
	}
	init_philos(sim);
	return (true);
}

static inline bool	init_value(char *str, int *value)
{
	int64_t	val;
	char	endptr;

	val = ft_strtol(str, &endptr);
	if (endptr || val > INT_MAX || val < INT_MIN)
	{
		ft_error("Invalid input.");
		return (false);
	}
	*value = (int)val;
	return (true);
}

static inline bool	validate_sim_args(const t_sim *sim, const int argc)
{
	if (sim->num_philos < 1 || sim->num_philos > MAX_PHILOS)
		ft_error("Invalid number of philosophers.");
	if (sim->time_to_die < MIN_TIME)
		ft_error("Invalid time to die.");
	if (sim->time_to_eat < MIN_TIME)
		ft_error("Invalid time to eat.");
	if (sim->time_to_sleep < MIN_TIME)
		ft_error("Invalid time to sleep.");
	if (argc == 6 && sim->num_meals < 1)
		ft_error("Invalid number of meals.");
	if (\
sim->num_philos < 1 || sim->num_philos > MAX_PHILOS || \
sim->time_to_die < MIN_TIME || \
sim->time_to_eat < MIN_TIME || \
sim->time_to_sleep < MIN_TIME || \
(argc == 6 && sim->num_meals < 1))
	{
		return (false);
	}
	return (true);
}
