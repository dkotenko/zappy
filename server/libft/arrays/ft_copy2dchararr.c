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

#include "libft_basic.h"

char	**ft_copy2dchararr(char **src)
{
	int		len_2d;
	char	**new;
	int		i;

	len_2d = len_2dchararr_terminated(src);
	new = ft_memalloc(sizeof(char *) * (len_2d + 1));
	i = -1;
	while (src[++i])
	{
		new[i] = ft_strdup(src[i]);
	}
	return (new);
}
