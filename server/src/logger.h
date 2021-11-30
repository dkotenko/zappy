/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 17:01:17 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 13:15:31 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include <stdio.h>
# include <sys/time.h>

void log_init(void);
void log_tick(struct timeval *select_timeout);
void log_debug(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
void log_info(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
void log_warning(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
void log_error(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
void log_fatal(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

#endif
