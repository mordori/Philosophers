/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 03:29:22 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 01:21:57 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_UTILS_H
# define STRING_UTILS_H

# include "defines.h"

size_t		uint64_to_str(uint64_t n, char *buf);
size_t		int_to_str(int n, char *buf);
int64_t		str_to_int64(char *str, char *end);
uint64_t	str_to_uint64(char *str, char *end);

#endif
