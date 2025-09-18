/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:25:54 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 22:29:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logging.h"
#include "timing.h"
#include "philosopher.h"
#include "simulation.h"
#include "string_utils.h"

static inline void	make_batch(t_queue *q);
static inline void	flush_batch(t_queue *q);

void	log_state(t_philo *p, const t_state state)
{
	int			next;
	t_queue		*q;
	static char	*states[] = \
{"is thinking", "has taken a fork", "is eating", "is sleeping", "died"};

	q = p->sim->queue;
	pthread_mutex_lock(&q->mutex);
	if (q->done)
	{
		pthread_mutex_unlock(&q->mutex);
		return ;
	}
	next = (q->head + 1) % QUEUE_SIZE;
	if (next != q->tail)
	{
		q->logs[q->head].timestamp = time_now() - p->sim->time_start;
		q->logs[q->head].philo_id = p->id;
		q->logs[q->head].state = states[state];
		q->head = next;
	}
	if (state == dead)
		q->done = true;
	pthread_mutex_unlock(&q->mutex);
}

void	*logging(void *arg)
{
	t_queue	*q;

	q = (t_queue *)arg;
	pthread_mutex_lock(&q->mutex);
	q->init = true;
	pthread_mutex_unlock(&q->mutex);
	while (true)
	{
		pthread_mutex_lock(&q->mutex);
		if (q->done && q->head == q->tail)
		{
			pthread_mutex_unlock(&q->mutex);
			break ;
		}
		make_batch(q);
		pthread_mutex_unlock(&q->mutex);
		if (q->count > 0)
			flush_batch(q);
		else
			usleep(1000);
	}
	return (NULL);
}

static inline void	make_batch(t_queue *q)
{
	q->count = 0;
	while (q->tail != q->head && q->count < QUEUE_BATCH_SIZE)
	{
		q->batch[q->count++] = q->logs[q->tail];
		q->tail = (q->tail + 1) % QUEUE_SIZE;
	}
}

static inline void	flush_batch(t_queue *q)
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
