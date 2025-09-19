/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 03:28:40 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/19 02:26:36 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string_utils.h"

static inline bool	ft_isdigit(int c);

/**
 * @brief Converts a string to an unsigned 64-bit integer.
 *
 * @param str Input string.
 * @param end Pointer to a character where the function stores the next
 * character after the parsed number. If parsing succeeds, it is set to '\0'.
 *
 * @return Parsed unsigned 64-bit integer value. Returns 0 on failure or if
 * the number is out of range.
 */
uint64_t	str_to_uint64(char *str, char *end)
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

/**
 * @brief Converts a string to a signed 64-bit integer.
 *
 * @param str Input string.
 * @param end Pointer to a character where the function stores the next
 * character after the parsed number. If parsing succeeds, it is set to '\0'.
 *
 * @return Parsed signed 64-bit integer value. Returns 0 on failure or if
 * the number is out of range.
 */
int64_t	str_to_int64(char *str, char *end)
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
			return (0);
		}
		number = number * 10 + (*str++ - '0');
		*end = *str;
	}
	return (sign * number);
}

/**
 * @brief Converts an unsigned 64-bit integer to a string.
 *
 * @param n Unsigned 64-bit integer to convert.
 * @param buf Buffer where the string result is written.
 *
 * @return The number of characters written.
 */
size_t	uint64_to_str(uint64_t n, char *buf)
{
	char	temp[INT64_LENGTH];
	size_t	i;
	size_t	j;

	if (!n)
	{
		buf[0] = '0';
		return (1);
	}
	i = 0;
	while (n > 0)
	{
		temp[i++] = '0' + (n % 10);
		n /= 10;
	}
	j = 0;
	while (j < i)
	{
		buf[j] = temp[i - j - 1];
		++j;
	}
	return (i);
}

/**
 * @brief Converts an signed 64-bit integer to a string.
 *
 * @param n Signed 64-bit integer to convert.
 * @param buf Buffer where the string result is written.
 *
 * @return The number of characters written.
 */
size_t	int_to_str(int n, char *buf)
{
	char	temp[INT32_LENGTH];
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	if (!n)
		buf[i++] = '0';
	if (n < 0)
		buf[i++] = '-';
	if (n < 0)
		n = -n;
	k = 0;
	while (n > 0)
	{
		temp[k++] = '0' + (n % 10);
		n /= 10;
	}
	j = 0;
	while (j < k)
	{
		buf[i + j] = temp[k - j - 1];
		++j;
	}
	return (i + k);
}

/**
 * Checks if `c` is a numeric character.
 *
 * @param c Character to be checked for.
 *
 * @return True if successful, else false.
 */
static inline bool	ft_isdigit(int c)
{
	c = (unsigned char)c;
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}
