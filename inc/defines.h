/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:13:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 02:18:29 by myli-pen         ###   ########.fr       */
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
typedef struct s_sim	t_sim;
typedef struct s_fork	t_fork;
typedef struct s_philo	t_philo;

#endif
