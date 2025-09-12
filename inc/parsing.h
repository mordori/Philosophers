/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:04:47 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/10 22:54:15 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <pthread.h>

# include "defines.h"

bool	parse_uint64(char *str, uint64_t *value);
bool	parse_int(char *str, int64_t *value);

#endif
