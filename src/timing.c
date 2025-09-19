/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:49:55 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 05:20:37 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timing.h"
#include "simulation_utils.h"

/**
 * @brief Sleeps the process for a specified duration in milliseconds.
 *
 * Waiting is done in chuncks to check whether the simulation is active.
 *
 * @note Suitable for longer sleeping.
 *
 * @param duration Time in milliseconds to be waited.
 * @param sim Pointer to the simulation.
 */
void	wait_until(uint64_t duration, t_sim *sim)
{
	uint64_t	time_end;

	time_end = time_now() + duration;
	while (time_now() < time_end && is_active(sim))
	{
		if (time_end - time_now() > 6)
			usleep(LONG_SLEEP_TIME);
		else
			usleep(SLEEP_TIME);
	}
}

/**
 * @brief Sleeps the process for a specified duration in milliseconds.
 *
 * Waiting is done in small chuncks to improve precision.
 *
 * @note Suitable for short sleeps.
 *
 * @param duration Time in milliseconds to be waited.
 */
void	wait_ms(uint64_t duration)
{
	uint64_t	time_end;

	time_end = time_now() + duration;
	while (time_now() < time_end)
		usleep(SLEEP_TIME);
}

/**
 * @brief Returns the current time.
 *
 * @return Time in microseconds since Epoch.
 */
uint64_t	time_now(void)
{
	struct timeval	tv;
	uint64_t		time;

	gettimeofday(&tv, NULL);
	time = (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
	return (time);
}
