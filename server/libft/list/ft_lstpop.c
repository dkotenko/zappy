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

#include <signal.h>
#include "libft.h"

static void show_list(t_list *alst)
{
	t_list *tmp = alst;
	int count = 0;
	if (!tmp)
		printf("[]");
	while (tmp) {
		printf("[%p]", tmp->content);
		tmp = tmp->next;
		if (++count == 100) {
			printf("\n");
			raise(SIGSEGV);
		}
	}
	printf("\n");
}

t_list	*ft_lstpop(t_list **list, void *value)
{
	t_list	*temp;
	t_list	*prev;
	
	printf("ft_lstpop(%p)\n", value);
	if (!(*list))
		return NULL;
	prev = *list;
	if (!ft_memcmp(prev->content, value, prev->content_size))
	{
		*list = (*list)->next;
		prev->next = NULL;
		show_list(*list);
		return prev;
	}
	temp = (*list)->next;
	while (temp)
	{
		if (!ft_memcmp(temp->content, value, temp->content_size))
		{
			prev->next = temp->next;
			temp->next = NULL;
			show_list(*list);
			return temp;
		}
		prev = temp;
		temp = temp->next;
	}
	show_list(*list);
	return NULL;
}
