/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:26:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 04:55:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGING_H
# define LOGGING_H

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

struct s_log
{
	uint64_t	timestamp;
	int			philo_id;
	char		*state;
};

struct s_queue
{
	t_log			logs[QUEUE_SIZE];
	t_log			batch[QUEUE_BATCH_SIZE];
	char			buf[QUEUE_BUFFER_SIZE];
	pthread_mutex_t	mutex;
	int				count;
	volatile int	head;
	volatile int	tail;
	volatile bool	done;
	bool			init;
	t_sim			*sim;
};

void	log_state(t_philo *p, const t_state state);
void	*logging(void *arg);

#endif
