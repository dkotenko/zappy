/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:43 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/21 18:06:10 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CIRCBUF_H
# define CIRCBUF_H

# include <stdlib.h>

extern int g_circbuf_debug;		/* set to 1 for additional debug */

typedef struct s_circbuf {
	size_t data_size; /* size of 1 element */
	size_t size;	  /* number of elements */
	char *buf;		  
	size_t buf_size;  /* data_size * size */
	size_t iread;
	size_t iwrite;
	size_t len;
} t_circbuf;


/* Allocates circular buffer with `size' items `data_size' bytes each */
t_circbuf circbuf_init(size_t size, size_t data_size);

/* Clears circular buffer */
void circbuf_clear(t_circbuf *circbuf);

/* Push one item from `data' to circular buffer */
void circbuf_push(t_circbuf *circbuf, void *data);

/* Push string to circular buffer */
void circbuf_push_string(t_circbuf *circbuf, char *str);

/* Pop one item from circular buffer. Use free() after work */
void *circbuf_pop(t_circbuf *circbuf);

/* Pop all items and concatenate items as string. Use free() after work */
char *circbuf_pop_string(t_circbuf *circbuf);

#endif
