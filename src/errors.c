/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:24:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/22 05:56:01 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "simulation.h"
#include "logging.h"
#include "philosopher.h"

/**
 * @brief Prints an error message to STDERR.
 *
 * @param msg Error message to display.
 */
void	ft_perror(char *msg)
{
	size_t	len;
	int		bytes;

	len = 0;
	while (msg[len])
		++len;
	bytes = write(STDERR_FILENO, "Error: ", 7);
	bytes = write(STDERR_FILENO, msg, len);
	bytes = write(STDERR_FILENO, "\n", 1);
	(void)bytes;
}

/**
 * @brief Flags an error occurrance in the simulation and prints an error
 * message.
 *
 * @param msg Error message to display.
 *
 * @return False to signal forward the error.
 */
bool	error_parse(char *msg)
{
	ft_perror(msg);
	return (false);
}

/**
 * @brief Flags an error occurrance in the simulation and prints an error
 * message.
 *
 * @param sim Pointer to the simulation.
 */
void	error_sim(t_sim *sim)
{
	ft_perror("Failed to create a thread.");
	pthread_mutex_lock(&sim->mutex_active);
	sim->error = true;
	pthread_mutex_unlock(&sim->mutex_active);
}

/**
 * @brief The log queue reached capacity before it could be flushed. Stops
 * the logging and prints an error message.
 *
 * @note Caller has locked the queue mutex so writing to the variables is safe.
 *
 * @param sim Pointer to the queue.
 */
void	error_logging(t_queue *q)
{
	ft_perror("QUEUE_SIZE is insufficient.");
	q->done = true;
	q->bytes = ERROR;
}
