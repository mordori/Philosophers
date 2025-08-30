/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:11:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 05:49:29 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "simulation.h"

static inline void	print_state(t_philo *philo);

void	init_philos(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philos)
	{
		sim->philos[i].id = i;
		sim->philos[i].sim = sim;
		sim->philos[i].state = i % ALIVE;
		++i;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	philo;

	philo = *(t_philo *)arg;
	while (philo.state < DEAD)
	{
		if (philo.state == THINKING)
		{
			if (philo.time > philo.sim->time_to_die)
			{
				philo.state = DEAD;

			}
		}
		else if (philo.state == EATING)
		{

		}
		else if (philo.state == SLEEPING)
		{

		}
	}
	return (NULL);
}

static inline void	print_state(t_philo *philo)
{

}
