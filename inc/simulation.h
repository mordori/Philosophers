/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:57:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/22 06:59:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>

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
	t_queue				*queue;
	t_philo				*philos;
	t_fork				*forks;
	pthread_mutex_t		mutex_active;
	pthread_t			thread_queue;
	int					threads_running;
	int					threads_odd_init;
	bool				active;
	bool				error;
	int64_t				philos_dined;
	uint64_t			time_start;
	int					num_fork_mutex_init;
	bool				mutex_active_init;
	bool				mutex_queue_init;
};

void	simulate(t_sim *sim);
bool	init_sim(t_sim *sim, const int argc, char *argv[]);

#endif
