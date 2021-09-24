/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:46:44 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/24 17:24:59 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include "circbuf.h"

typedef struct s_queue {
	t_circbuf circbuf;
} t_queue;

t_queue queue_init(size_t size, size_t data_size);
void queue_push(t_queue *queue, void *data);
void *queue_pop(t_queue *queue);
void queue_clear(t_queue *queue);

#endif
