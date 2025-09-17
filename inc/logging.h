/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:26:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 03:26:29 by myli-pen         ###   ########.fr       */
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
	char		state[STATE_LENGTH];
};

struct s_queue
{
	t_log			buffer[LOG_QUEUE_SIZE];
	int				head;
	int				tail;
	pthread_mutex_t	mutex;
	bool			done;
};

void	*logging(void *arg);

#endif
