/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:57:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 05:42:32 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# include "defines.h"
# include "errors.h"
# include "philo.h"
# include "utils.h"

struct s_sim
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	t_philo			*philos;
	pthread_mutex_t	lock;
	int				active;
};

bool	simulate(t_sim *sim);
bool	init_sim(t_sim *sim, const int argc, char *argv[]);

#endif
