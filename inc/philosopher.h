/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:21:16 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/12 03:55:58 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# include "defines.h"

struct s_fork
{
	pthread_mutex_t	mutex;
};

struct s_philo
{
	t_sim				*sim;
	t_fork				*fork_l;
	t_fork				*fork_r;
	int					id;
	pthread_t			thread;
	volatile int64_t	meals;
	volatile uint64_t	time_last_meal;
};

void	*philo_routine(void *arg);

#endif
