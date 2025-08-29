/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:13:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 20:26:20 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdbool.h>
# include <stdint.h>
# include <limits.h>

# define ERROR -1
# define MAX_PHILOS 200
# define MIN_TIME 60

typedef enum e_state	t_state;
typedef struct s_config	t_config;
typedef struct s_fork	t_fork;
typedef struct s_philo	t_philo;

#endif
