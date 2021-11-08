/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intmatrixnew.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 13:40:24 by clala             #+#    #+#             */
/*   Updated: 2019/12/08 13:40:26 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_arr(int **arr, int i)
{
	while (i > -1)
		free(arr[i--]);
	free(arr);
}

int	**ft_intmatrixnew(int rows, int cols)
{
	int		**matrix;
	int		i;

	matrix = (int **)ft_memalloc(sizeof(int *) * rows);
	i = 0;
	while (i < rows)
	{
		matrix[i] = ft_intarrnew(cols);
		if (!matrix[i])
		{
			free_arr(matrix, i - 1);
			return (NULL);
		}
		i++;
	}
	return (matrix);
}
