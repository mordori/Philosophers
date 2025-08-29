/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:45:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 20:33:06 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

static inline bool	init_value(char *str, int *value);
static inline bool	validate_config(const t_config *config, const int argc);

bool	init_config(t_config *config, const int argc, char *argv[])
{
	if (\
!init_value(argv[1], &config->philos) || \
!init_value(argv[2], &config->time_to_die) || \
!init_value(argv[3], &config->time_to_eat) || \
!init_value(argv[4], &config->time_to_sleep))
	{
		ft_error("Invalid input.");
		return (false);
	}
	config->meals = -1;
	if (argc == 6 && !init_value(argv[5], &config->meals))
	{
		ft_error("Invalid input.");
		return (false);
	}
	if (!validate_config(config, argc))
		return (false);
	return (true);
}

static inline bool	init_value(char *str, int *value)
{
	int64_t	val;
	char	endptr;

	val = ft_strtol(str, &endptr);
	if (endptr || val > INT_MAX || val < INT_MIN)
		return (false);
	*value = (int)val;
	return (true);
}

static inline bool	validate_config(const t_config *config, const int argc)
{
	if (config->philos < 1 || config->philos > MAX_PHILOS)
		ft_error("Invalid number of philosophers.");
	if (config->time_to_die < MIN_TIME)
		ft_error("Invalid time to die.");
	if (config->time_to_eat < MIN_TIME)
		ft_error("Invalid time to eat.");
	if (config->time_to_sleep < MIN_TIME)
		ft_error("Invalid time to sleep.");
	if (argc == 6 && config->meals < 1)
		ft_error("Invalid number of meals.");
	if (\
config->philos < 1 || config->philos > MAX_PHILOS || \
config->time_to_die < MIN_TIME || \
config->time_to_eat < MIN_TIME || \
config->time_to_sleep < MIN_TIME || \
(argc == 6 && config->meals < 1))
	{
		return (false);
	}
	return (true);
}
