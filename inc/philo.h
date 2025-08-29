/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:21:16 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 21:07:45 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include "defines.h"
# include "config.h"
# include "errors.h"

enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	MAX
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
	t_state		state;
	t_fork		fork_l;
	t_fork		fork_r;
};

void	init_philos(t_philo *philos, const t_config *congif);

#endif
