/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:19:52 by gmelisan          #+#    #+#             */
/*   Updated: 2021/10/17 16:29:29 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>

#include "utils.h"
#include "logger.h"
#include "server.h"

int xassert(int value, const char *str)
{
	if (!value) {
		log_fatal("%s: %s", str, strerror(errno));
	}
	return !value ? 1 : 0;
}


void timerprint(void (*f)(const char *format, ...), struct timeval *t, char *name)
{
	f("%s: {.tv_sec = %zu, .tv_usec = %zu}", name, t->tv_sec, t->tv_usec);
}

int str_starts_with(char *str, char *with)
{
	if (strncmp(str, with, strlen(with)) == 0)
		return 1;
	return 0;
}
