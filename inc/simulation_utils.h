/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:00:19 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/12 02:44:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_UTILS_H
# define SIMULATION_UTILS_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

# include "defines.h"

bool		is_active(t_sim *sim);
void		clean_sim(t_sim *sim, pthread_mutex_t *mutex_print, \
pthread_mutex_t *mutex_meal);
void		init_philos(t_sim *sim);
void		wait_ms(uint64_t duration, t_sim *sim);
uint64_t	time_now(void);

#endif
