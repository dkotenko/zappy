/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:51 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/21 17:21:31 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "circbuf.h"
#include "logger.h"
#include "utils.h"

int g_circbuf_debug;

static void circbuf_debug_print(t_circbuf *circbuf)
{
	char *buf = (char *)malloc(circbuf->data_size + 1);
	xassert(buf != NULL, "malloc");
	memset(buf, 0, circbuf->data_size + 1);
	for (size_t i = 0; i < circbuf->size; ++i) {
		memcpy(buf, circbuf->buf + (i * circbuf->data_size), circbuf->data_size);
		log_debug("%zu [%s]", i, buf);
	}
	free(buf);
}

t_circbuf circbuf_init(size_t size, size_t data_size)
{
	t_circbuf circbuf;

	memset(&circbuf, 0, sizeof(circbuf));
	circbuf.data_size = data_size;
	circbuf.size = size;
	circbuf.buf_size = data_size * size;
	circbuf.buf = (char *)malloc(circbuf.buf_size);
	xassert(circbuf.buf != NULL, "malloc");
	memset(circbuf.buf, 0, circbuf.buf_size);
	if (g_circbuf_debug)
		log_debug("circbuf_init: allocate buffer[%zu * %zu]", data_size, size);
	return circbuf;
}

void circbuf_clear(t_circbuf *circbuf)
{
	free(circbuf->buf);
	log_debug("circbuf_clear");
}

void circbuf_push(t_circbuf *circbuf, void *data)
{
	if (circbuf->len == circbuf->size)
		log_warning("Circular buffer lost data");
	memcpy(&circbuf->buf[circbuf->iwrite], data, circbuf->data_size);
	++circbuf->len;
	circbuf->iwrite += circbuf->data_size;
	if (circbuf->iwrite >= circbuf->buf_size)
		circbuf->iwrite = 0;
	if (circbuf->len > circbuf->len)
		circbuf->len = circbuf->len;
	if (g_circbuf_debug) {
		log_debug("circbuf_push: iwrite = %zu, len = %zu",
				  circbuf->iwrite, circbuf->len);
		circbuf_debug_print(circbuf);
	}		 
}

void circbuf_push_string(t_circbuf *circbuf, char *str)
{
	int i = 0;
	int len = strlen(str);
	while (i + circbuf->data_size <= len) {
		circbuf_push(circbuf, str + i);
		i += circbuf->data_size;
	}
	char *buf = (char *)malloc(circbuf->data_size);
	xassert(buf != NULL, "malloc");
	strcpy(buf, str + i);
	circbuf_push(circbuf, buf);
	free(buf);
}


// data_size = 5
// .....,,,,,@@@@
// 012345678901
// hello,world!



void *circbuf_pop(t_circbuf *circbuf)
{
	void *pop_buf;

	if (circbuf->len == 0)
		return NULL;
	pop_buf = malloc(circbuf->data_size);
	xassert(pop_buf != NULL, "malloc");
	memcpy(pop_buf, circbuf->buf + circbuf->iread, circbuf->data_size);
	--circbuf->len;
	circbuf->iread += circbuf->data_size;
	if (circbuf->iread >= circbuf->buf_size)
		circbuf->iread = 0;
	if (g_circbuf_debug) {
		log_debug("circbuf_pop: iread = %zu, len = %zu",
				  circbuf->iread, circbuf->len);
		circbuf_debug_print(circbuf);
	}
	return pop_buf;
}

char *circbuf_pop_string(t_circbuf *circbuf)
{
	char *pop_buf;
	int i = 0;
	
	pop_buf = (char *)malloc(circbuf->buf_size + 1);
	xassert(pop_buf != NULL, "malloc");
	memset(pop_buf, 0, circbuf->buf_size + 1);
	while (circbuf->len) {
		void *item = circbuf_pop(circbuf);
		memcpy(pop_buf + i, item, circbuf->data_size);
		free(item);
		i += circbuf->data_size;
	}
	if (g_circbuf_debug) {
		log_debug("circbuf_pop_all: return '%s'", pop_buf);
	}
	return pop_buf;
}
