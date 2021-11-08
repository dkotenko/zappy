/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:43:37 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 21:53:21 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_free(char *format, ...)
{
	int		n;
	char	*s;
	va_list	ap;

	va_start(ap, format);
	n = ft_atoi((const char *)format);
	while (n-- > 0)
	{
		s = va_arg(ap, char *);
		free((void *)s);
	}
	va_end(ap);
	return (1);
}

int	ft_free_null(void **to_free)
{
	ft_free_int(*to_free);
	*to_free = NULL;
	return (1);
}

int	ft_free_int(void *to_free)
{
	if (!to_free)
		return (1);
	free(to_free);
	return (1);
}
