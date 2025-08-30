/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:21:16 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 05:50:04 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include "defines.h"
# include "errors.h"

enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	ALIVE,
	DEAD
};

struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
};

struct s_philo
{
	int			id;
	pthread_t	thread;
	int			time;
	int			meals;
	t_sim		*sim;
	t_state		state;
	t_fork		fork_l;
	t_fork		fork_r;
};

void	init_philos(t_sim *sim);
void	*philo_routine(void *arg);

#endif
