/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2dchararr_terminated.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:43:37 by clala             #+#    #+#             */
/*   Updated: 2021/02/07 00:06:19 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	free_2dchararr_terminated(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr[i]);
	free(arr);
	return (1);
}

int	len_2dchararr_terminated(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

int	print_2dchararr_terminated(char **arr)
{
	while (*arr)
	{
		ft_putstr(*arr);
		ft_putstr("\n");
		arr++;
	}
	return (1);
}
