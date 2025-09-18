/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:13:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/18 21:58:01 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdbool.h>
# include <stdint.h>
# include <limits.h>
# include <stddef.h>

# define ERROR				-1

# define SPIN_TIME			250		// μs
# define SLEEP_TIME			5000	// μs

# define QUEUE_SIZE			1024	// t_log
# define QUEUE_BATCH_SIZE	256		// t_log
# define QUEUE_BUFFER_SIZE	16384	// byte
# define LOG_LENGTH			64		// byte

typedef enum e_state		t_state;

typedef struct s_config		t_config;
typedef struct s_fork		t_fork;
typedef struct s_philo		t_philo;
typedef struct s_sim		t_sim;
typedef struct s_queue		t_queue;
typedef struct s_log		t_log;

#endif
