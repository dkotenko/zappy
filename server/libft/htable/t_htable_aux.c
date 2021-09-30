/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_htable2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:47:48 by clala             #+#    #+#             */
/*   Updated: 2020/02/15 14:33:59 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_htable.h"

int	cmp_func(const void *a, const void *b)
{
	return (ft_strcmp((const char *)a, (const char *)b));
}

int	is_prime_number(int n)
{
	int			i;

	if (n == 1)
		return (0);
	i = 2;
	while (i * i <= n)
	{
		if (!(n % i))
			return (0);
		i++;
	}
	return (1);
}

int	get_prime_size(int size)
{
	while (!is_prime_number(size))
		size++;
	return (size);
}

void	t_htable_free(t_htable *table)
{
	int			i;

	i = -1;
	while (++i < table->counter)
	{
		free(table->table[table->curr_data[i]]);
	}
	free(table->curr_data);
	free(table->table);
	free(table);
}
