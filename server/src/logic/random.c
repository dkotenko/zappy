#include "zappy.h"

/*
 *	FROM 0 to max_number
 *	UPPER BOUND NOT INCLUDED
*/

int	get_random_in_range(int max_number)
{
	return rand() % max_number;
}

t_cell	*get_random_cell(t_map *map)
{	
	return map->cells[get_random_in_range(map->h)]\
	[get_random_in_range(map->w)];
}

/*
 *	FROM min_num to max_num
 *	UPPER BOUND NOT INCLUDED
*/
int	get_random_from_to(int min_num, int max_num)
{
	return min_num + (rand() % (max_num - min_num));
}
