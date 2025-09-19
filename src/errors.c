/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:24:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 05:22:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "simulation.h"

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
 * message to STDERR.
 *
 * @param sim Pointer to the simulation.
 */
void	sim_error(t_sim *sim)
{
	ft_perror("Failed to create a thread.");
	pthread_mutex_lock(&sim->mutex_active);
	sim->error = true;
	pthread_mutex_unlock(&sim->mutex_active);
}
