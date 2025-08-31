/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:00:19 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/31 03:16:22 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_UTILS_H
# define SIMULATION_UTILS_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

#include "defines.h"

void	monitor_philos(t_sim *sim);
void	set_with_mutex(int *var, int val, pthread_mutex_t *mutex);
int		get_with_mutex(const int var, pthread_mutex_t *mutex);
int64_t	time_now(void);
void	clean_sim(t_sim *sim, pthread_mutex_t *active, pthread_mutex_t *print);

#endif
