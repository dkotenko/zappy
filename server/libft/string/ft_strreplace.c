/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 12:49:53 by clala             #+#    #+#             */
/*   Updated: 2021/04/24 21:30:28 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strreplace(char *s, char *what, char *to)
{
	char	*new;
	int		i;
	char	*entry;
	char	*start_new;

	if (!s || !what || !to || !(ft_strlen(what)))
		return (NULL);
	i = 0;
	entry = ft_strstr(s, what);
	while (entry && ++i > -1)
		entry = ft_strstr(entry + ft_strlen(what), what);
	new = ft_strnew(ft_strlen(s) + (ft_strlen(to) - ft_strlen(what)) * i);
	start_new = new;
	entry = ft_strstr(s, what);
	while (i-- && entry)
	{
		ft_strncpy(start_new, s, entry - s);
		start_new += entry - s;
		ft_strcpy(start_new, to);
		s = entry + ft_strlen(what);
		start_new += ft_strlen(to);
		entry = ft_strstr(s, what);
	}
	ft_strcpy(start_new, s);
	return (new);
}
