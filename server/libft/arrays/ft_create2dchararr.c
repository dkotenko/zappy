/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create2dchararr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:43:37 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 22:02:50 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_create2dchararr(int rows, int cols)
{
	char	**new;
	int		i;

	new = (char **)ft_memalloc(sizeof(char *) * (rows + 1));
	i = 0;
	while (i < rows)
	{
		new[i] = ft_strnew(cols);
		if (!new[i])
		{
			ft_free2dchararr(new, i);
			return (NULL);
		}
		i++;
	}
	return (new);
}
