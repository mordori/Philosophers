/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:00:19 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 20:30:43 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_UTILS_H
# define SIMULATION_UTILS_H

# include <pthread.h>
# include <stdlib.h>

# include "defines.h"

bool		is_active(t_sim *sim);
void		clean_sim(t_sim *sim,
				pthread_mutex_t *mutex_print,
				pthread_mutex_t *mutex_active,
				pthread_mutex_t *mutex_queue);
void		init_philos(t_sim *sim);

#endif
