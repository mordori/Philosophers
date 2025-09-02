/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:57:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/01 23:02:45 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# include "defines.h"

struct s_config
{
	int64_t	num_philos;
	int64_t	num_meals;
	int64_t	time_to_die;
	int64_t	time_to_eat;
	int64_t	time_to_sleep;
};

struct s_sim
{
	t_config					config;
	t_philo						*philos;
	t_fork						*forks;
	int							num_philo_mutex_init;
	int							num_fork_mutex_init;
	pthread_mutex_t				mutex_print;
	volatile _Atomic int		philos_init;
	volatile _Atomic int64_t	philos_dined;
	volatile _Atomic bool		start;
	volatile _Atomic bool		active;
	volatile _Atomic int64_t	time_start;
};

bool	init_sim(t_sim *sim, const int argc, char *argv[]);
void	simulate(t_sim *sim);

#endif
