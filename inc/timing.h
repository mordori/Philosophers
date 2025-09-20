/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:50:06 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/20 17:39:28 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMING_H
# define TIMING_H

# include <unistd.h>
# include <sys/time.h>

# include "defines.h"

uint64_t	time_now(void);
void		wait_until(uint64_t end, t_sim *sim);

#endif
