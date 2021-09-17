/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:51 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/17 17:35:20 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "circbuf.h"
#include "logger.h"

void circbuf_push1(t_circbuf *circbuf, char data)
{
	if (circbuf->len == CIRCBUF_SIZE)
		log_warning("Circular buffer lost data (%c)",
					circbuf->buf[circbuf->iwrite]);
	circbuf->buf[circbuf->iwrite] = data;
	++circbuf->len;
	++circbuf->iwrite;
	if (circbuf->iwrite == CIRCBUF_SIZE)
		circbuf->iwrite = 0;
	if (circbuf->len > CIRCBUF_SIZE) {
		circbuf->len = CIRCBUF_SIZE;
	}
}

// TODO circbuf_push memcpy
void circbuf_push(t_circbuf *circbuf, char *data, size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		circbuf_push1(circbuf, data[i]);
	}
}

char circbuf_pop1(t_circbuf *circbuf)
{
	char c;

	c = circbuf->buf[circbuf->iread];
	--circbuf->len;
	--circbuf->iread;
	if (circbuf->iread == CIRCBUF_SIZE)
		circbuf->iread = 0;
	return c;
}

// TODO circbuf_pop memcpy
char *circbuf_pop(t_circbuf *circbuf)
{
	static char buf[CIRCBUF_SIZE + 1];
	int i = 0;
	
	while (circbuf->len) {
		buf[i++] = circbuf_pop1(circbuf);
	}
	buf[i] = 0;
	return buf;
}
