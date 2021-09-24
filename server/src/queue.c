/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:55:39 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/24 17:24:54 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"

t_queue queue_init(size_t size, size_t data_size)
{
	t_queue queue;
	queue.circbuf = circbuf_init(size, data_size);
	return queue;
}
void queue_push(t_queue *queue, void *data)
{
	if (queue->circbuf.size == queue->circbuf.len)
		return ;
	circbuf_push(&queue->circbuf, data);
}
void *queue_pop(t_queue *queue)
{
	return circbuf_pop(&queue->circbuf);
}
void queue_clear(t_queue *queue)
{
	circbuf_clear(&queue->circbuf);
}
