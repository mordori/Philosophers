/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:21:16 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 20:03:21 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# define _GNU_SOURCE

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# include "defines.h"

struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
};

struct s_philo
{
	int			id;
	pthread_t	thread;
	int			meals;
	int64_t		time_last_meal;
	t_sim		*sim;
	t_fork		*fork_l;
	t_fork		*fork_r;
};

void	*philo_routine(void *arg);

#endif
