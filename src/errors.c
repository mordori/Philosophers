/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:24:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/29 20:09:39 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"

ssize_t	ft_putstr_fd(char *s, int fd);
ssize_t	ft_putendl_fd(char *s, int fd);

/**
 * @brief Handles errors by printing a message and exiting.
 *
 * @param msg Error message to display.
 */
void	ft_error(char *msg)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

/**
 * Outputs string `s` to the specified file descriptor.
 *
 * @param s Source string.
 * @param fd File descriptor.
 * @return Number of characters written, ERROR on error or only partial write.
 */
ssize_t	ft_putstr_fd(char *s, int fd)
{
	ssize_t	bytes;
	ssize_t	totalbytes;
	size_t	len;

	if (!s)
		return (ERROR);
	totalbytes = 0;
	len = ft_strlen(s);
	while (len > 0)
	{
		bytes = write (fd, s, len);
		if (bytes < 0)
			return (ERROR);
		totalbytes += bytes;
		len -= bytes;
	}
	return (totalbytes);
}

/**
 * Outputs string `s` to the specified file descriptor followed by a newline.
 *
 * @param s Source string.
 * @param fd File descriptor.
 * @return Number of characters written, ERROR on error or only partial write.
 */
ssize_t	ft_putendl_fd(char *s, int fd)
{
	ssize_t	bytes;

	if (!s)
		return (ERROR);
	bytes = ft_putstr_fd(s, fd);
	if (bytes == ERROR || write (fd, "\n", 1) == ERROR)
		return (ERROR);
	return (bytes + 1);
}
