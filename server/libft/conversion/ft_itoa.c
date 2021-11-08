/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 13:47:03 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 21:53:21 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		sign;
	char	*s;
	int		len;
	int		i;

	len = ft_ilen(n);
	sign = n;
	s = ft_strnew(len + 1);
	i = 0;
	while (n)
	{
		s[i++] = ft_abs(n % 10) + '0';
		n /= 10;
	}
	if (sign < 0)
		s[i++] = '-';
	else if (sign == 0)
		s[i++] = '0';
	s[i] = '\0';
	ft_strrev(s);
	return (s);
}
