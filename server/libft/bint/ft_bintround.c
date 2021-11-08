/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bintround.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:25:04 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 21:53:21 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_bank_rnd(char *decimal, int prec)
{
	int		i;

	if (ft_ctoi(decimal[prec + 1]) == 5)
	{
		i = 1;
		while (decimal[prec + 1 + i])
		{
			if (ft_ctoi(decimal[prec + 1 + i++]) > 0)
				return (1);
			if (i > 14)
				break ;
		}
		if (decimal[prec] == '.' && ft_ctoi(decimal[prec - 1]) % 2)
			return (1);
		else if (ft_ctoi(decimal[prec] % 2))
			return (1);
	}
	return (0);
}

static int	get_prec(int prec)
{
	if (prec < 0)
		prec = 6;
	return (prec);
}

static char	*ft_bintround(char *num, int prec)
{
	int32_t	decimals;
	char	*decpt;

	num = ft_strdup(num);
	prec = get_prec(prec);
	decpt = ft_strchr(num, '.');
	if (decpt)
	{
		decimals = (ft_strlen(num) - 1) - (decpt - num);
		if (decimals > prec || prec == 0)
		{
			if (is_bank_rnd(decpt, prec) || ft_ctoi(*(decpt + prec + 1)) > 5)
				num = ft_bintaddtn(num, ft_ldtoa(ft_pow(10, -prec), prec));
		}
		else
			num = ft_strjoinfree(num, ft_strnewchr(prec - decimals, '0'), 1, 1);
		if (prec)
			num[(ft_strchr(num, '.') - num) + prec + 1] = '\0';
		else
			num[(ft_strchr(num, '.') - num) + 0] = '\0';
	}
	else if (prec)
		num = ft_strjoinfree(ft_strjoinfree(num, ".", 1, 0),
				ft_strnewchr(prec, '0'), 1, 1);
	return (num);
}

char	*ft_bintroundfree(char *num, int precision, int free_num)
{
	char	*str;

	str = ft_bintround(num, precision);
	if (free_num && num)
		free((void *)num);
	return (str);
}
