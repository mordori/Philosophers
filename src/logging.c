/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:25:54 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/22 14:43:50 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logging.h"
#include "timing.h"
#include "philosopher.h"
#include "simulation.h"
#include "simulation_utils.h"
#include "string_utils.h"
#include "errors.h"

static inline void	make_batch(t_queue *q);
static inline void	flush_batch(t_queue *q);

/**
 * @brief Logging thread routine.
 *
 * Constructs batches from the recorded logs and flushes them until the queue
 * has been emptied.
 *
 * @note Performed periodically every 1 millisecond.
 *
 * @param arg Pointer to the queue containing the logs.
 *
 * @return Null pointer.
 */
void	*logging(void *arg)
{
	t_queue	*q;

	q = (t_queue *)arg;
	pthread_mutex_lock(&q->mutex);
	q->init = true;
	pthread_mutex_unlock(&q->mutex);
	while (!all_threads_running(q->sim))
		usleep(SPIN_TIME);
	while (true)
	{
		pthread_mutex_lock(&q->mutex);
		if ((q->done && q->head == q->tail) || q->bytes == ERROR)
		{
			pthread_mutex_unlock(&q->mutex);
			break ;
		}
		make_batch(q);
		if (q->count > 0)
			flush_batch(q);
		pthread_mutex_unlock(&q->mutex);
		if (!q->count)
			usleep(SLEEP_TIME);
	}
	return (NULL);
}

/**
 * @brief Constructs a log and adds it to the queue.
 *
 * @note Flags logging to be done if a philosopher died.
 *
 * @param p Philosopher containing it's id.
 * @param state State of the philosopher to be logged.
 */
void	log_state(t_philo *p, const t_state state)
{
	int			next;
	t_queue		*q;
	static char	*states[] = \
{"is thinking", "has taken a fork", "is eating", "is sleeping", "died"};

	q = p->sim->queue;
	pthread_mutex_lock(&q->mutex);
	if (q->done || q->bytes == ERROR)
	{
		pthread_mutex_unlock(&q->mutex);
		return ;
	}
	next = (q->head + 1) % QUEUE_SIZE;
	if (next != q->tail)
	{
		q->logs[q->head].timestamp = (time_now() - p->sim->time_start) / 1000;
		q->logs[q->head].philo_id = p->id;
		q->logs[q->head].state = states[state];
		q->head = next;
	}
	else
		error_logging(q);
	if (state == dead)
		q->done = true;
	pthread_mutex_unlock(&q->mutex);
}

/**
 * @brief Flags the logging to be done.
 *
 * Called when the simulation has been ended from the monitoring thread.
 *
 * @param q Pointer to the queue.
 */
void	stop_logging(t_queue *q)
{
	pthread_mutex_lock(&q->mutex);
	q->done = true;
	pthread_mutex_unlock(&q->mutex);
}

/**
 * @brief Constructs a batch with logs from the queue and moves the queue tail
 * index.
 *
 * @param q Pointer to the queue.
 */
static inline void	make_batch(t_queue *q)
{
	q->count = 0;
	while (q->tail != q->head && q->count < QUEUE_BATCH_SIZE)
	{
		q->batch[q->count++] = q->logs[q->tail];
		q->tail = (q->tail + 1) % QUEUE_SIZE;
	}
}

/**
 * @brief Converts the logs from a batch to a buffered string and writes it to
 * STDOUT.
 *
 * @param q Pointer to the queue.
 */
static inline void	flush_batch(t_queue *q)
{
	size_t	offset;
	char	*s;
	int		i;

	offset = 0;
	i = 0;
	while (i < q->count && q->bytes != ERROR)
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
			q->bytes = write(STDOUT_FILENO, q->buf, offset);
			offset = 0;
		}
		++i;
	}
	if (offset > 0 && q->bytes != ERROR)
		q->bytes = write(STDOUT_FILENO, q->buf, offset);
}
