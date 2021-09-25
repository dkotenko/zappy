/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:19:52 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/25 20:59:51 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>

#include "utils.h"
#include "logger.h"
#include "server.h"

void xassert(int value, const char *str)
{
	if (!value) {
		log_fatal("%s: %s", str, strerror(errno));
	}
}

void timerprint(void (*f)(const char *format, ...), struct timeval *t, char *name)
{
	f("%s: {.tv_sec = %zu, .tv_usec = %zu}", name, t->tv_sec, t->tv_usec);
}
