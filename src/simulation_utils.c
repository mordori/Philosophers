/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:56:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 20:45:09 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation_utils.h"
#include "simulation.h"

void	set_with_mutex(int *var, int val, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	*var = val;
	pthread_mutex_unlock(mutex);
}

int	get_with_mutex(const int var, pthread_mutex_t *mutex)
{
	int	result;

	pthread_mutex_lock(mutex);
	result = var;
	pthread_mutex_unlock(mutex);
	return (result);
}

int64_t	time_now(void)
{
	struct timeval	tv;
	int64_t			result;

	gettimeofday(&tv, NULL);
	result = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	return (result);
}

void	free_sim(t_sim *sim)
{
	free(sim->philos);
	free(sim->forks);
}
