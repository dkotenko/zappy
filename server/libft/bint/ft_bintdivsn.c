/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bintdivsn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:24:29 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 21:53:21 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	initializer(int *carry, int *i, char **n, char **s)
{
	*carry = 0;
	*i = 0;
	*s = *n;
}

static char	*handle_dot_carry(char *n, int i, int carry)
{
	if (n[i] != '.' && carry)
		n = ft_strjoinfree(n, ".", 1, 0);
	return (n);
}

static char	*handle_endline_carry(char *n, int i, int carry)
{
	if (carry && n[i] == '\0')
		n = ft_strjoinfree(n, "0", 1, 0);
	return (n);
}

char	*ft_bintdivsn(char *n, int divider)
{
	int		carry;
	int		sum;
	int		i;
	char	*s;

	initializer(&carry, &i, &n, &s);
	n = ft_strdup(n);
	while (n[i] && n[i] != '.')
	{
		sum = carry + ft_ctoi(n[i]);
		n[i++] = ft_itoc(sum / divider);
		carry = sum % divider * 10;
	}
	n = handle_dot_carry(n, i, carry);
	i += (n[i] == '.');
	while (n[i] || carry)
	{
		n = handle_endline_carry(n, i, carry);
		sum = carry + ft_ctoi(n[i]);
		n[i++] = ft_itoc(sum / divider);
		carry = sum % divider * 10;
	}
	if (s)
		free((void *)s);
	return (ft_bintfmt(n));
}
