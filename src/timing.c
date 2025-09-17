/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:49:55 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 02:56:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timing.h"
#include "simulation_utils.h"

void	wait_until(uint64_t duration, t_sim *sim)
{
	uint64_t	time_end;

	time_end = time_now() + duration;
	while (time_now() < time_end && is_active(sim))
	{
		if (time_end - time_now() > 6)
			usleep(SLEEP_TIME);
		else
			usleep(SPIN_TIME);
	}
}

void	wait_ms(uint64_t duration)
{
	uint64_t	time_end;

	time_end = time_now() + duration;
	while (time_now() < time_end)
		usleep(SPIN_TIME);
}

uint64_t	time_now(void)
{
	struct timeval	tv;
	uint64_t		time;

	gettimeofday(&tv, NULL);
	time = (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
	return (time);
}
