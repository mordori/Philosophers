/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:24:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 21:10:36 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "simulation.h"

/**
 * @brief Handles errors by printing a message and exiting.
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

void	sim_error(t_sim *sim)
{
	ft_perror("Failed to create a thread.");
	pthread_mutex_lock(&sim->mutex_active);
	sim->error = true;
	pthread_mutex_unlock(&sim->mutex_active);
}
