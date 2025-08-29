/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:46:43 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 20:33:00 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <stdio.h>

# include "defines.h"
# include "errors.h"

struct s_config
{
	int	philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meals;
};

bool	init_config(t_config *config, const int argc, char *argv[]);

#endif
