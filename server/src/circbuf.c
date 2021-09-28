/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:51 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 18:29:31 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "circbuf.h"
#include "logger.h"
#include "utils.h"

static void circbuf_debug_print(t_circbuf *circbuf)
{
	char *buf = (char *)malloc(circbuf->data_size + 1);
	xassert(buf != NULL, "malloc");
	memset(buf, 0, circbuf->data_size + 1);
	for (size_t i = 0; i < circbuf->size; ++i) {
		memcpy(buf, circbuf->buf + (i * circbuf->data_size), circbuf->data_size);
		log_debug("%p %zu [%s]", circbuf, i, buf);
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
	//circbuf.debug = 1;
	return circbuf;
}

void circbuf_clear(t_circbuf *circbuf)
{
	free(circbuf->buf);
	if (circbuf->debug)
		log_debug("%p circbuf_clear", circbuf);
}

void circbuf_push(t_circbuf *circbuf, void *data)
{
	if (circbuf->debug) {
		log_debug("%p circbuf_push: iwrite = %zu", circbuf,
				  circbuf->iwrite / circbuf->data_size);
	}
	if (circbuf->len == circbuf->size)
		log_warning("Circular buffer %p lost data", circbuf);
	memcpy(circbuf->buf + circbuf->iwrite, data, circbuf->data_size);
	++circbuf->len;
	circbuf->iwrite += circbuf->data_size;
	if (circbuf->iwrite >= circbuf->buf_size)
		circbuf->iwrite = 0;
	if (circbuf->len > circbuf->size)
		circbuf->len = circbuf->size;
	if (circbuf->debug) {
		log_debug("%p circbuf_push: iwrite = %zu, len = %zu", circbuf,
				  circbuf->iwrite / circbuf->data_size, circbuf->len);
		circbuf_debug_print(circbuf);
	}		 
}


/* TODO push to previous item as well if has space */
void circbuf_push_string(t_circbuf *circbuf, char *str)
{
	size_t i = 0;
	size_t len = strlen(str);
	char *buf;

	if (circbuf->debug)
		log_debug("%p circbuf_push_string", circbuf);
	
	while (i + circbuf->data_size <= len) {
		circbuf_push(circbuf, str + i);
		i += circbuf->data_size;
	}
	buf = (char *)malloc(circbuf->data_size);
	memset(buf, 0, circbuf->data_size);
	xassert(buf != NULL, "malloc");
	strcpy(buf, str + i);
	circbuf_push(circbuf, buf);
	free(buf);
}

void *circbuf_pop(t_circbuf *circbuf)
{
	void *pop_buf;

	if (circbuf->len == 0)
		return NULL;
	if (circbuf->debug) {
		log_debug("%p circbuf_pop: iread = %zu", circbuf,
				  circbuf->iread / circbuf->data_size);
	}
	
	pop_buf = malloc(circbuf->data_size);
	xassert(pop_buf != NULL, "malloc");
	memcpy(pop_buf, circbuf->buf + circbuf->iread, circbuf->data_size);
	--circbuf->len;
	circbuf->iread += circbuf->data_size;
	if (circbuf->iread >= circbuf->buf_size)
		circbuf->iread = 0;
	if (circbuf->debug) {
		log_debug("%p circbuf_pop: iread = %zu, len = %zu", circbuf,
				  circbuf->iread / circbuf->data_size, circbuf->len);
		circbuf_debug_print(circbuf);
	}
	return pop_buf;
}

char *circbuf_pop_string(t_circbuf *circbuf)
{
	char *pop_buf;
	void *item;
	char *p;

	if (circbuf->debug)
		log_debug("%p circbuf_pop_string", circbuf);
	
	pop_buf = (char *)malloc(circbuf->buf_size + 1);
	p = pop_buf;
	xassert(pop_buf != NULL, "malloc");
	memset(pop_buf, 0, circbuf->buf_size + 1);
	while (circbuf->len) {
		item = circbuf_pop(circbuf);
		p = stpncpy(p, item, circbuf->data_size);
		free(item);
	}
	return pop_buf;
}
