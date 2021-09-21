/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:43 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/21 15:00:12 by gmelisan         ###   ########.fr       */
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

/* Pop one item from circular buffer. Use free() after work */
void *circbuf_pop(t_circbuf *circbuf);

/* Pop all items and concatenate items as string. Use free() after work */
char *circbuf_pop_all(t_circbuf *circbuf);

/*
Usage example:
size = 5
 0  1  2  3  4
[ ][ ][ ][ ][ ]		iwrite = 0, iread = 0, len = 0

 push a
 [a][ ][ ][ ][ ]		iwrite = 1, iread = 0, len = 1

 push b
 [a][b][ ][ ][ ]		iwrite = 2, iread = 0, len = 2			free_space = 3, size = 5

 push c
 [a][b][c][ ][ ]		iwrite = 3, iread = 0, len = 3

 pop -> a
 [a][b][c][ ][ ]		iwrite = 3, iread = 1, len = 2

 pop -> b
 [a][b][c][ ][ ]		iwrite = 3, iread = 2, len = 1

 push d, push e, push f
 [f][b][c][d][e]		iwrite = 1, iread = 2, len = 5

 pop -> c
 [f][b][c][d][e]		iwrite = 1, iread = 3, len = 5
*/

#endif
