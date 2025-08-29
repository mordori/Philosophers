/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:57:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 20:51:23 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include <stdlib.h>

# include "defines.h"
# include "config.h"
# include "errors.h"
# include "philo.h"

bool	simulate(const t_config *config);

#endif
