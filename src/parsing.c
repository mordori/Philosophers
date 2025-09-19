/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:58:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 02:32:43 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "string_utils.h"

/**
 * @brief Parses and validates a signed 64-bit integer.
 *
 * @param str String to be parsed.
 * @param value Pointer to the variable that the parsed result is assigned to.
 *
 * @return Success of the performed parsing operation.
 */
bool	parse_int(char *str, int64_t *value)
{
	int64_t	result;
	char	endptr;

	result = str_to_int64(str, &endptr);
	if (endptr)
		return (false);
	*value = result;
	return (true);
}

/**
 * @brief Parses and validates a unsigned 64-bit integer.
 *
 * @param str String to be parsed.
 * @param value Pointer to the variable that the parsed result is assigned to.
 *
 * @return Success of the performed parsing operation.
 */
bool	parse_uint64(char *str, uint64_t *value)
{
	uint64_t	result;
	char		endptr;

	result = str_to_uint64(str, &endptr);
	if (endptr)
		return (false);
	*value = result;
	return (true);
}
