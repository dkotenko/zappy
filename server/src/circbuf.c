/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:51 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/16 21:53:26 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "circbuf.h"

void circbuf_push1(t_circbuf circbuf, char data)
{
	circbuf.buf[iwrite] = data;
	++circbuf.len;
	++circbuf.iwrite;
	if (circbuf.iwrite == CIRCBUF_SIZE)
		circbuf.iwrite = 0;
	if (circbuf.len > CIRCBUF_SIZE)
		circbuf.len = CIRCBUF_SIZE;
}

void circbuf_push(t_circbuf circbuf, char *data, size_t size)
{
	/* size_t free_space = CIRCBUF_SIZE - circbuf.iwrite; */

	/* while (size > free_space) { */
	/* 	memcpy(&circbuf.buf[iwrite], data, s); */
	/* } */
}

char circbuf_pop1(t_circbuf circbuf)
{
	char c;

	c = circbuf.buf[iread];
	--circbuf.len;
	--circbuf.iread;
	if (readIndex == CIRCBUF_SIZE)
		circbuf.iread = 0;	
}

char *circbuf_pop(t_circbuf circbuf)
{

}
