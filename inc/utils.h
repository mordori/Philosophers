/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:04:47 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 03:44:31 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <pthread.h>

# include "defines.h"

int64_t	ft_strtol(char *str, char *end);
size_t	ft_strlen(const char *s);
void	set_with_mutex(int *var, int val, pthread_mutex_t *mutex);

#endif
