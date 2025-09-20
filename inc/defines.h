/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:13:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/20 22:05:14 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdbool.h>
# include <stdint.h>
# include <limits.h>
# include <stddef.h>

# define SPIN_TIME			50		// μs
# define SLEEP_TIME			1000	// μs

# define QUEUE_SIZE			1024	// t_log
# define QUEUE_BATCH_SIZE	256		// t_log
# define QUEUE_BUFFER_SIZE	16384	// byte
# define LOG_LENGTH			64		// byte

# define INT32_LENGTH		11		// byte
# define INT64_LENGTH		20		// byte

typedef enum e_state		t_state;

typedef struct s_config		t_config;
typedef struct s_fork		t_fork;
typedef struct s_philo		t_philo;
typedef struct s_sim		t_sim;
typedef struct s_queue		t_queue;
typedef struct s_log		t_log;

#endif
