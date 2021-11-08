/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_htable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:47:48 by clala             #+#    #+#             */
/*   Updated: 2021/03/13 21:33:29 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/t_htable.h"

t_htable	*t_htable_resize(t_htable *table)
{
	t_htable	*new;
	int			next_size;
	int			i;

	next_size = get_prime_size(table->size * 2);
	new = t_htable_create(next_size);
	new->hash = table->hash;
	new->cmp = table->cmp;
	i = -1;
	while (++i < table->real_size)
	{
		if (table->table[i])
		{
			t_htable_add(&new, table->table[i]->key, \
			table->table[i]->value);
		}
	}
	t_htable_free(table);
	return (new);
}

int	t_htable_add(t_htable **table, void *key, void *value)
{
	int			i;
	t_hash		hash;
	t_hash		size;

	if ((*table)->counter >= (*table)->size / 2)
		*table = t_htable_resize(*table);
	hash = (*table)->hash(key, (*table)->size);
	i = 0;
	size = (*table)->real_size;
	while (i + hash < size)
	{
		if (!(*table)->table[hash + i])
		{
			(*table)->table[hash + i] = t_htable_data_create(key, value);
			(*table)->curr_data[(*table)->counter] = hash + i;
			(*table)->counter++;
			return (1);
		}
		else if (!((*table)->cmp(((*table)->table[hash + i])->key, key)))
			return (0);
		i++;
	}
	*table = t_htable_resize(*table);
	t_htable_add(table, key, value);
	return (0);
}

t_htable	*t_htable_init(int size, t_htable_cmp *cmp, t_htable_hash *hash)
{
	t_htable	*t;

	t = t_htable_create(size);
	if (!t)
		return (t);
	t->cmp = cmp;
	t->hash = hash;
	return (t);
}

t_htable	*t_htable_create(int size)
{
	t_htable	*new;

	new = (t_htable *)ft_memalloc(sizeof(t_htable));
	if (size)
		size = get_prime_size(size);
	else
		size = get_prime_size(T_HTABLE_INIT_PRIME_NUMBER);
	new->real_size = get_prime_size(size + T_HTABLE_REAL_SIZE_OFFSET);
	new->table = (t_htable_data **)ft_memalloc(\
		sizeof(t_htable_data *) * new->real_size);
	new->size = size;
	new->curr_data = (t_hash *)ft_memalloc(sizeof(t_hash) * new->real_size);
	new->counter = 0;
	return (new);
}
