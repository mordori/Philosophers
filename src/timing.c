/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:49:55 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/22 14:40:45 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timing.h"
#include "simulation_utils.h"

/**
 * @brief Sleeps the process for a specified duration in microseconds.
 *
 * Waiting is done in chuncks to check whether the simulation is active.
 *
 * @note Suitable for longer sleeping.
 *
 * @param duration Time in microseconds to be waited.
 * @param sim Pointer to the simulation.
 */
void	wait_until(uint64_t end, t_sim *sim)
{
	uint64_t	now;
	uint64_t	remaining;
	uint64_t	duration;

	while (time_now() < end && is_active(sim))
	{
		now = time_now();
		if (now >= end)
			break ;
		remaining = end - now;
		if (remaining > SLEEP_TIME * 100)
			duration = SLEEP_TIME * 100;
		else if (remaining > SLEEP_TIME * 50)
			duration = SLEEP_TIME * 50;
		else if (remaining > SLEEP_TIME * 20)
			duration = SLEEP_TIME * 20;
		else if (remaining > SLEEP_TIME * 5)
			duration = SLEEP_TIME * 5;
		else
			duration = SLEEP_TIME;
		if (duration > remaining)
			duration = remaining;
		usleep(duration);
	}
}

/**
 * @brief Returns the current time in microseconds.
 *
 * @return Time in microseconds since Epoch.
 */
uint64_t	time_now(void)
{
	struct timeval	tv;
	uint64_t		time;

	gettimeofday(&tv, NULL);
	time = (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
	return (time);
}
