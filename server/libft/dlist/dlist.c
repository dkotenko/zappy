/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 22:05:39 by clala             #+#    #+#             */
/*   Updated: 2021/02/13 21:56:37 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dlist.h"

t_dlist	*t_dlist_new(void)
{
	t_dlist		*new;

	new = (t_dlist *)ft_memalloc(sizeof(t_dlist));
	new->size = 0;
	new->head = NULL;
	new->tail = NULL;
	return (new);
}

t_dlist_node	*t_dlist_pop(t_dlist *list, t_dlist_node *node)
{
	if (!node->prev)
		list->head = node->next;
	else
		node->prev->next = node->next;
	if (!node->next)
		list->tail = node->prev;
	else
		node->next->prev = node->prev;
	if (node->next == node && node->prev == node)
	{
		list->head = NULL;
		list->tail = NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_dlist	*t_dlist_remove_node(t_dlist *list, t_dlist_node *node)
{
	t_dlist_pop(list, node);
	free(node);
	return (list);
}

t_dlist_node	*t_dlist_get(t_dlist *list, void *value)
{
	t_dlist_node	*tmp;

	tmp = list->head;
	while (tmp != NULL)
	{
		if (!ft_memcmp(tmp->data, value, tmp->data_size))
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}
