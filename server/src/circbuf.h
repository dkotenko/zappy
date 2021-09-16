/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circbuf.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 21:07:43 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/16 21:53:19 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CIRCBUF_H
# define CIRCBUF_H

# define CIRCBUF_SIZE		128

typedef struct s_circbuf {
	char buf[CIRCBUF_SIZE];
	int iread;
	int iwrite;
	int len;
} t_circbuf;

/* push one byte */
void circbuf_push1(t_circbuf circbuf, char data);

/* push `size' bytes from `data' array */
void circbuf_push(t_circbuf circbuf, char *data, size_t size);

/* pop one byte, return 0 if no data */
size_t circbuf_pop1(t_circbuf circbuf, char *data);

/* pop all data to `data', return poped size, 0 if no data */
size_t circbuf_pop(t_circbuf circbuf, char **data);


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
