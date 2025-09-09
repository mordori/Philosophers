/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:13:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/09 21:48:12 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdbool.h>
# include <stdint.h>
# include <limits.h>
# include <stddef.h>

# ifndef ERROR
#  define ERROR -1
# endif
# ifndef SUCCESS
#  define SUCCESS 1
# endif
# ifndef FAILURE
#  define FAILURE 0
# endif
# ifndef EXIT
#  define EXIT -1
# endif
# define MAX_PHILOS 200
# define MIN_TASK_TIME 60
# define SPIN_TIME 250

typedef struct s_config	t_config;
typedef struct s_fork	t_fork;
typedef struct s_philo	t_philo;
typedef struct s_sim	t_sim;

#endif
