/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 17:04:14 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/16 17:45:45 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#include "server.h"
#include "logger.h"

#define COLOR_GRAY		"\e[1;30m"
#define COLOR_RED		"\e[0;31m"
#define COLOR_GREEN		"\e[0;32m"
#define COLOR_YELLOW	"\e[0;33m"
#define COLOR_RESET		"\e[0m"


static char *get_time()
{
	time_t rawtime;
	struct tm *timeinfo;
	static char buf[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, 80, "%d.%m.%Y %H:%M:%S", timeinfo);
	return buf;
}

static void log_print(FILE *f, const char *format, va_list ap, const char *prefix)
{
	if (g_main_config.quiet)
		return ;
	fprintf(f, "[%s] ", get_time());
	fprintf(f, "[%s] ", prefix);
	vfprintf(f, format, ap);
	fprintf(f, "\n");
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
