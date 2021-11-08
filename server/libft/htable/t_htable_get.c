/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_htable_get.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:47:48 by clala             #+#    #+#             */
/*   Updated: 2021/04/24 21:55:18 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/t_htable.h"

/*
** returns pointer to a value
*/

void	*t_htable_get(t_htable *table, void *key)
{
	t_hash	hash;

	hash = t_htable_find(table, key);
	if (!hash)
		return (NULL);
	return (table->table[hash]->value);
}

/*
** returns an array of pointers to values
*/

void	**t_htable_get_values(t_htable *table)
{
	void	**new;
	int		i;
	t_hash	hash;

	new = (void **)ft_memalloc(sizeof(void *) * (table->counter + 1));
	i = -1;
	while (++i < table->counter)
	{
		hash = table->curr_data[i];
		new[i] = table->table[hash]->value;
	}
	return (new);
}

/*
** returns an array of pointers to keys
*/

void	**t_htable_get_keys(t_htable *table)
{
	void	**new;
	int		i;
	t_hash	hash;

	new = (void **)ft_memalloc(sizeof(void *) * (table->counter + 1));
	i = -1;
	while (++i < table->counter)
	{
		hash = table->curr_data[i];
		new[i] = table->table[hash]->key;
	}
	return (new);
}

/*
** returns an array of arrays of pointers to keys and values
*/

void	***t_htable_get_keys_values(t_htable *table)
{
	void	***new;
	int		i;
	t_hash	hash;
	void	**keys;
	void	**values;

	new = (void ***)ft_memalloc(sizeof(void **) * table->counter);
	keys = t_htable_get_keys(table);
	values = t_htable_get_values(table);
	i = -1;
	while (++i < table->counter)
	{
		hash = table->curr_data[i];
		new[i] = (void **)ft_memalloc(sizeof(void *) * 3);
		new[i][T_HTABLE_KEY] = keys[i];
		new[i][T_HTABLE_VALUE] = values[i];
	}
	free(keys);
	free(values);
	return (new);
}
