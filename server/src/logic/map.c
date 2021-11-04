#include "zappy.h"



t_cell    *create_cell(int x, int y)
{
     t_cell    *new;

     new = (t_cell *)ft_memalloc(sizeof(t_cell));
     new->x = x;
     new->y = y;
     new->inventory = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
     return (new);
}

t_map	*create_map(t_game *game, int w, int h)
{
     t_map     	*map;

     map = (t_map *)ft_memalloc(sizeof(t_map));
     map->w = w;
     map->h = h;
     map->cells = (t_cell ***)ft_memalloc(sizeof(t_cell **) * (h));
     for (int i = 0; i < map->h; i++)
     {
		map->cells[i] = (t_cell **)ft_memalloc(sizeof(t_cell *) * (w));
		for (int j = 0; j < map->w; j++) {
			map->cells[i][j] = create_cell(j, i);
		}
     }
     game->map = map;
     return (map);
}

void	init_random_map(t_game *game)
{
	//init food 
	int food_amount = game->aux->incantation_sum[RESOURCES_NUMBER_OF_PLAYERS] * \
		game->teams_num * /*R_RICH * */ R_LEVEL;
	while (food_amount--) {
		get_random_cell(game->map)->inventory[NOURRITURE]++;
	}

	//init other resources
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		int amount = game->aux->incantation_sum[i] * R_LEVEL;
		while (amount--) {
			get_random_cell(game->map)->inventory[i]++;
		}
	}
}

void	init_logic_map(t_game *game)
{
	game = NULL;
}

void	mock_init_map(t_game *game)
{
	game = NULL;
}

void	clear_resources_map(t_game *game)
{
	t_map *map = game->map;
	
	for (int i = 0; i < map->h; i++)
     {
		for (int j = 0; j < map->w; j++) {
			ft_memset(map->cells[i][j]->inventory, 0, sizeof(int));
		}
     }
}