/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 13:55:46 by clala             #+#    #+#             */
/*   Updated: 2021/10/10 16:45:43 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstpop(t_list **list, void *value)
{
	t_list	*temp;
	t_list	*prev;
	
	if (!(*list))
		return NULL;
	prev = *list;
	if (!ft_memcmp(prev->content, value, prev->content_size))
	{
		*list = (*list)->next;
		return prev;
	}
	temp = (*list)->next;
	while (temp)
	{
		if (!ft_memcmp(temp->content, value, temp->content_size))
		{
			prev->next = temp->next;
			temp->next = NULL;
			return temp;
		}
		prev = temp;
		temp = temp->next;
	}
	return NULL;
}
