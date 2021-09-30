/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:47:48 by clala             #+#    #+#             */
/*   Updated: 2021/02/07 00:04:51 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/t_htable.h"

unsigned long long	hash_func_fnv_1a_64(void *key, int size)
{
	unsigned char		*p;
	unsigned long long	h;
	int					i;
	int					len;

	p = key;
	len = ft_strlen((char *)p);
	h = 0xcbf29ce484222325ULL;
	i = -1;
	while (++i < len)
	{
		h = (h ^ p[i]) * 0x100000001b3ULL;
	}
	if (h == 0)
		h = 1;
	return (h % size);
}

unsigned int	hash_func_fnv_1a_32(void *key, int size)
{
	unsigned char		*p;
	unsigned int		h;
	int					i;
	int					len;

	h = 0x811c9dc5;
	p = key;
	len = ft_strlen((char *)p);
	i = -1;
	while (++i < len)
	{
		h = (h ^ p[i]) * 0x01000193;
	}
	if (h == 0)
		h = 1;
	return (h % size);
}

unsigned int	hash_func_kernighan_ritchie(void *p, int size)
{
	int					i;
	uintmax_t			hash;
	int					len;
	char				*s;

	s = (char *)p;
	hash = 1;
	len = ft_strlen(s);
	i = 0;
	while (i < len)
	{
		hash *= (((hash * (uintmax_t)T_HTABLE_INIT_PRIME_NUMBER) \
			+ (uintmax_t)s[i]) % (uintmax_t)size);
		i++;
	}
	if (hash == 0)
		hash = 1;
	if (hash < 0)
		return ((hash % size) * -1);
	return (hash % size);
}
