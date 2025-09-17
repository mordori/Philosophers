/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:25:54 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 01:17:38 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logging.h"
#include "timing.h"
#include "string_utils.h"

static inline bool	is_done(t_queue *q);

void	*logging(void *arg)
{
	t_queue	*q;

	q = (t_queue *)arg;
	while (!is_done(q) || q->head != q->tail)
	{
		q->count = 0;
		pthread_mutex_lock(&q->mutex);
		while (q->tail != q->head && q->count < QUEUE_BATCH_SIZE)
		{
			q->batch[q->count++] = q->logs[q->tail];
			q->tail = (q->tail + 1) % QUEUE_SIZE;
		}
		pthread_mutex_unlock(&q->mutex);
		if (q->count > 0)
			flush_queue(q);
		else
			usleep(1000);
	}
	return (NULL);
}

void	flush_queue(t_queue *q)
{
	size_t	offset;
	char	*s;
	int		i;

	offset = 0;
	i = 0;
	while (i < q->count)
	{
		offset += uint64_to_str(q->batch[i].timestamp, q->buf + offset);
		q->buf[offset++] = ' ';
		offset += int_to_str(q->batch[i].philo_id, q->buf + offset);
		q->buf[offset++] = ' ';
		s = q->batch[i].state;
		while (*s)
			q->buf[offset++] = *s++;
		q->buf[offset++] = '\n';
		if (offset >= QUEUE_BUFFER_SIZE - LOG_LENGTH)
		{
			write(STDOUT_FILENO, q->buf, offset);
			offset = 0;
		}
		++i;
	}
	if (offset > 0)
		write(STDOUT_FILENO, q->buf, offset);
}

static inline bool	is_done(t_queue *q)
{
	bool	result;

	pthread_mutex_lock(&q->mutex);
	result = q->done;
	pthread_mutex_unlock(&q->mutex);
	return (result);
}
