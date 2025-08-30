/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:58:42 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/30 20:09:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string_utils.h"
#include "errors.h"

static inline int64_t	ft_strtol(char *str, char *end);
static inline bool	ft_isspace(char c);
static inline bool	ft_isdigit(int c);

bool	parse_int(char *str, int *value)
{
	int64_t	val;
	char	endptr;

	val = ft_strtol(str, &endptr);
	if (endptr || val > INT_MAX || val < INT_MIN)
	{
		ft_perror("Invalid input.");
		return (false);
	}
	*value = (int)val;
	return (true);
}

/**
 * Calculates the amount of characters in string `s`.
 *
 * @param s Source string.
 * @return The length of the string `s`.
 */
size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (false);
	len = 0;
	while (*s++)
		++len;
	return (len);
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
