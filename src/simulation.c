/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 20:55:54 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

bool	simulate(const t_config *config)
{
	t_philo *philos;

	philos = malloc(config->philos);
	if (!philos)
	{
		ft_error("Malloc philos.");
		return (false);
	}
	init_philos(philos, config);
	return (true);
}
