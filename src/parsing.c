/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:58:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/03 19:56:27 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static inline bool	ft_isdigit(int c);
static inline bool	ft_isspace(char c);
static inline int64_t	ft_strtol(char *str, char *end);

bool	parse_int(char *str, int64_t *value)
{
	int64_t	result;
	char	endptr;

	result = ft_strtol(str, &endptr);
	if (endptr || result > INT_MAX || result < INT_MIN)
		return (false);
	*value = result;
	return (true);
}

static inline int64_t	ft_strtol(char *str, char *end)
{
	int		sign;
	int64_t	number;

	*end = 'e';
	number = 0;
	if (!str)
		return (number);
	while (ft_isspace(*str))
		++str;
	sign = 1;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	while (ft_isdigit(*str))
	{
		number = number * 10 + (*str++ - '0');
		*end = *str;
	}
	return (sign * number);
}

/**
 * Checks if `c` is a whitespace character.
 *
 * @param c Character to be checked for.
 * @return TRUE if successful, else FALSE.
 */
static inline bool	ft_isspace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
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
