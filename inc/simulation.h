/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:57:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/31 03:13:53 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# define _GNU_SOURCE

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# include "defines.h"

struct s_config
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	num_meals;
};

struct s_sim
{
	t_config		config;
	t_philo			*philos;
	t_fork			*forks;
	int				philos_init;
	int				forks_init;
	pthread_mutex_t	mutex_active;
	pthread_mutex_t	mutex_print;
	int				start;
	int				active;
	int64_t			time_start;
};

void	simulate(t_sim *sim);
bool	init_sim(t_sim *sim, const int argc, char *argv[]);

#endif
