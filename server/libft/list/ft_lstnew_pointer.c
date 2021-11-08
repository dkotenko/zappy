/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 13:53:12 by clala             #+#    #+#             */
/*   Updated: 2021/09/24 19:34:30 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew_pointer(void const *content, size_t content_size)
{
	t_list	*listnew;

	if (content_size + 1 < content_size)
		return (NULL);
	listnew = (t_list *)ft_memalloc(sizeof(t_list));
	if (content == NULL)
	{
		listnew->content = NULL;
		listnew->content_size = 0;
	}
	else
	{
		listnew->content = (void *)content;
		listnew->content_size = content_size;
	}
	listnew->next = NULL;
	return (listnew);
}
