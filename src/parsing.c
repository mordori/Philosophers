/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:58:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/12 01:12:51 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static inline bool	ft_isdigit(int c);
static inline uint64_t	str_to_uint64(char *str, char *end);
static inline int64_t	str_to_int64(char *str, char *end);

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

static inline uint64_t	str_to_uint64(char *str, char *end)
{
	uint64_t	number;

	*end = 'e';
	number = 0;
	if (!str)
		return (number);
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		++str;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		return (0);
	while (ft_isdigit(*str))
	{
		if (number > ((UINT64_MAX - (*str - '0')) / 10))
		{
			*end = 'e';
			return (0);
		}
		number = number * 10 + (*str++ - '0');
		*end = *str;
	}
	return (number);
}

static inline int64_t	str_to_int64(char *str, char *end)
{
	int		sign;
	int64_t	number;

	*end = 'e';
	number = 0;
	if (!str)
		return (number);
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		++str;
	sign = 1;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	while (ft_isdigit(*str))
	{
		if (((sign == 1 && number > ((INT64_MAX - (*str - '0')) / 10))) || \
(sign == -1 && number > (((-(INT64_MIN + (*str - '0'))) / 10))))
		{
			*end = 'e';
			return (ERROR);
		}
		number = number * 10 + (*str++ - '0');
		*end = *str;
	}
	return (sign * number);
}

/**
 * Checks if `c` is a numeric character.
 *
 * @param c Character to be checked for.
 * @return True if successful, else false.
 */
static inline bool	ft_isdigit(int c)
{
	c = (unsigned char)c;
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}
