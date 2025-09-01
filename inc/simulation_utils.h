/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:00:19 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/01 18:30:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_UTILS_H
# define SIMULATION_UTILS_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

# include "defines.h"

void	monitor_philo_death(t_sim *sim);
void	wait_for(int64_t target, t_sim *sim);
int64_t	time_now(void);
void	clean_sim(t_sim *sim, pthread_mutex_t *print);
void	init_philos(t_sim *sim);

#endif
