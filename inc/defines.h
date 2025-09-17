/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:13:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/17 20:22:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdbool.h>
# include <stdint.h>
# include <limits.h>
# include <stddef.h>

# define ERROR				-1
# define START_TIME			15
# define SPIN_TIME			250
# define SLEEP_TIME			5000
# define MONITOR_TIME		5
# define LOG_QUEUE_SIZE		4096
# define LOG_BATCH_SIZE		256
# define LOG_BUFFER_SIZE	16384
# define STATE_LENGTH		64

typedef enum e_state	t_state;
typedef struct s_config	t_config;
typedef struct s_fork	t_fork;
typedef struct s_philo	t_philo;
typedef struct s_sim	t_sim;
typedef struct s_queue	t_queue;
typedef struct s_log	t_log;

#endif
