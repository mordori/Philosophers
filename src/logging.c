/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:25:54 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 04:23:26 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logging.h"
#include "timing.h"
#include "string_utils.h"

static inline bool	is_done(t_queue *q);

void	*logging(void *arg)
{
	t_queue	*q;
	t_log	batch[LOG_BATCH_SIZE];
	char	buf[LOG_BUFFER_SIZE];
	int		count;
	size_t	offset;
	int		i;
	char	*s;
	int a;

	q = (t_queue *)arg;
	while (!is_done(q) || q->head != q->tail)
	{
		pthread_mutex_lock(&q->mutex);
		count = 0;
		while (q->tail != q->head && count < LOG_BATCH_SIZE)
		{
			batch[count++] = q->buffer[q->tail];
			q->tail = (q->tail + 1) % LOG_QUEUE_SIZE;
		}
		pthread_mutex_unlock(&q->mutex);
		if (count > 0)
		{
			offset = 0;
			i = 0;
			while (i < count)
			{
				offset += uint64_to_str(batch[i].timestamp, buf + offset);
				buf[offset++] = ' ';
				offset += int_to_str(batch[i].philo_id, buf + offset);
				buf[offset++] = ' ';
				s = batch[i].state;
				while (*s && offset < LOG_BUFFER_SIZE - 1)
					buf[offset++] = *s++;
				buf[offset++] = '\n';
				if (offset >= LOG_BUFFER_SIZE - STATE_LENGTH)
				{
					a = write(STDOUT_FILENO, buf, offset);
					offset = 0;
				}
				++i;
			}
			if (offset > 0)
				a = write(STDOUT_FILENO, buf, offset);
		}
		else
			wait_ms(1);
	}
	(void)a;
	return (NULL);
}

static inline bool	is_done(t_queue *q)
{
	bool	result;

	pthread_mutex_lock(&q->mutex);
	result = q->done;
	pthread_mutex_unlock(&q->mutex);
	return (result);
}
