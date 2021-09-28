/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 17:04:14 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 14:46:57 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "logger.h"
#include "utils.h"

#define COLOR_GRAY		"\e[1;30m"
#define COLOR_RED		"\e[0;31m"
#define COLOR_GREEN		"\e[0;32m"
#define COLOR_YELLOW	"\e[0;33m"
#define COLOR_RESET		"\e[0m"


static char *get_time()
{
	struct timeval t;
	struct tm *timeinfo;
	static char buf[80];

	xassert(gettimeofday(&t, NULL) != -1, "gettimeofday");
	timeinfo = localtime(&t.tv_sec);
	strftime(buf, 80, "%d.%m.%Y %H:%M:%S", timeinfo);
	sprintf(strchr(buf, '\0'), ".%06d", t.tv_usec);
	return buf;
}

static int g_tick;

static void log_print(FILE *f, const char *format, va_list ap, const char *prefix)
{
	if (g_main_config.quiet)
		return ;
	if (g_tick) {
		fprintf(f, "\n");
		g_tick = 0;
	}
	fprintf(f, "[%s] ", get_time());
	fprintf(f, "[%s] ", prefix);
	vfprintf(f, format, ap);
	fprintf(f, "\n");
}

void log_tick(struct timeval *select_timeout)
{
#ifndef DEBUG
	return ;
#endif
	static int tick = 1;
	g_tick = 1;

	//if (tick == 10000)
	//	exit(0);

	fprintf(stderr, "[%s] tick %d (select timeout %zu.%06d)\r",
			get_time(), tick, select_timeout->tv_sec, select_timeout->tv_usec);
	++tick;
}

void log_debug(const char *format, ...)
{
#ifndef DEBUG
	(void)format;
	return ;
#endif
	va_list ap;

	va_start(ap, format);
	log_print(stdout, format, ap, "DEBUG");
	va_end(ap);
}

void log_info(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(stdout, format, ap, COLOR_GREEN "INFO" COLOR_RESET);
	va_end(ap);
}

void log_warning(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(stdout, format, ap, COLOR_YELLOW "WARNING" COLOR_RESET);
	va_end(ap);
}

void log_error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(stderr, format, ap, COLOR_RED "ERROR" COLOR_RESET);
	va_end(ap);
}

void log_fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(stderr, format, ap, COLOR_RED "FATAL" COLOR_RESET);
	va_end(ap);
	exit(1);
}
