/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 21:17:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos(t_philo *philos, const t_config *congif)
{
	int	i;

	i = 0;
	while (i < congif->philos)
	{
		philos[i].id = i;
		philos[i].state = i % MAX;
		if (pthread_create(philos[i].thread, NULL, ,NULL))
		{
			ft_error("Failed to create a thread.");
			break;
		}
	}
}
