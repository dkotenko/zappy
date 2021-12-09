/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 13:55:46 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 21:53:21 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "libft.h"

/*
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
*/
void	ft_lstadd(t_list **alst, t_list *new)
{
	if (new)
	{
		new->next = *alst;
		*alst = new;
	}
}
