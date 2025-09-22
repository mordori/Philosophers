/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:27:57 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/22 02:05:25 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <unistd.h>

# include "defines.h"

void	ft_perror(char *msg);
bool	error_parse(char *msg);
void	error_sim(t_sim *sim);
void	error_logging(t_queue *q);

#endif
