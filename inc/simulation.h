/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:57:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/14 03:51:23 by myli-pen         ###   ########.fr       */
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
	int64_t		num_philos;
	int64_t		num_meals;
	uint64_t	time_to_die;
	uint64_t	time_to_eat;
	uint64_t	time_to_sleep;
};

struct s_sim
{
	t_config			config;
	t_philo				*philos;
	t_fork				*forks;
	int					num_fork_mutex_init;
	pthread_mutex_t		mutex_active;
	pthread_mutex_t		mutex_print;
	volatile int64_t	philos_dined;
	volatile bool		active;
	uint64_t			time_start;
	uint64_t			monitor_wait_time;
};

void	simulate(t_sim *sim);
bool	init_sim(t_sim *sim, const int argc, char *argv[]);

#endif
