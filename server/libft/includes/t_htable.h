/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_htable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 12:07:29 by edrowzee          #+#    #+#             */
/*   Updated: 2021/03/13 14:26:30 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_HTABLE_H
# define T_HTABLE_H

# include "libft_basic.h"
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <fcntl.h>

# define T_HTABLE_INIT_PRIME_NUMBER 353
# define T_HTABLE_REAL_SIZE_OFFSET 10
# define T_HTABLE_KEY 0
# define T_HTABLE_VALUE 1

typedef unsigned int	t_hash;
typedef int				t_htable_cmp(const void *p1, const void *p2);
typedef t_hash			t_htable_hash(void *p, int size);

/*
** linked list struct for separate chaining collision resolution.
** Not implemented
*/
typedef struct s_hlist
{
	struct s_hlist		*next;
	void				*key;
	uintptr_t			val;
}						t_hlist;

/*
** htable element struct
*/
typedef struct s_htable_data
{
	void				*key;
	void				*value;
}						t_htable_data;

typedef struct s_htable {
	t_htable_cmp		*cmp;
	t_htable_hash		*hash;
	t_hash				*curr_data;
	int					counter;
	int					size;
	int					real_size;
	t_htable_data		**table;
}						t_htable;

/*
**	hash_functions.c
*/
unsigned long long		hash_func_fnv_1a_64(void *key, int size);
unsigned int			hash_func_fnv_1a_32(void *key, int size);
unsigned int			hash_func_kernighan_ritchie(void *p, int size);

/*
**	t_htable_aux.c
*/
int						cmp_func(const void*a, const void*b);
int						is_prime_number(int n);
int						get_prime_size(int size);
void					t_htable_free(t_htable *table);

/*
**	t_htable_methods.c
*/
t_hash					t_htable_find(t_htable *table, void *key);
int						t_htable_contains(t_htable *table, void *key);
void					*t_htable_get(t_htable *table, void *key);
int						t_htable_remove(t_htable *table, void *key);
int						t_htable_set(t_htable **table, void *key, void *value);

/*
**	t_htable_data.c
*/
t_htable_data			*t_htable_data_create(void *key, void *value);
void					t_htable_data_free(t_htable_data *data);
void					t_htable_clean_all(t_htable *table);

/*
**	t_htable.c
*/
t_htable				*t_htable_resize(t_htable *table);
int						t_htable_add(t_htable **table, void *key, void *value);
t_htable				*t_htable_create(int size);
t_htable				*t_htable_init(int size, t_htable_cmp *cmp, \
						t_htable_hash *hash);

/*
**	t_htable_get.c
*/
void					***t_htable_get_keys_values(t_htable *table);
void					**t_htable_get_keys(t_htable *table);
void					**t_htable_get_values(t_htable *table);
void					*t_htable_get(t_htable *table, void *key);
#endif
