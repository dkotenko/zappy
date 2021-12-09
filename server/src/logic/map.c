#include "zappy.h"
#include <math.h>



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

void	init_random_food(t_game *game)
{
	int food_amount = game->aux->incantation_sum[RESOURCES_NUMBER_OF_PLAYERS] * \
		game->teams_num * /*R_RICH * */ R_LEVEL;

	while (food_amount--) {
		get_random_cell(game->map)->inventory[NOURRITURE]++;
	}
}

/*
** all resources except NOURRITURE(food)
*/
void	init_random_resources(t_game *game)
{
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		int amount = game->aux->incantation_sum[i] * R_LEVEL;
		while (amount--) {
			get_random_cell(game->map)->inventory[i]++;
		}
	}
}

void	init_random_map(t_game *game)
{
	init_random_food(game);
	init_random_resources(game);
}

void	init_cluster_food(t_game *game)
{
	int curr_cell = 0;
	int food_amount = game->aux->incantation_sum[RESOURCES_NUMBER_OF_PLAYERS] * \
		game->teams_num * /*R_RICH * */ R_LEVEL;
	int map_size = game->map->h * game->map->w;
	int cluster_size = (int)ceil((double)map_size /(double)food_amount);

	while (food_amount) {
		int temp_x = curr_cell % game->map->w;
		int temp_y = curr_cell / game->map->w;
		game->map->cells[temp_y][temp_x]->inventory[NOURRITURE]++;
		food_amount--;
		curr_cell = (curr_cell + cluster_size) % (game->map->w * game->map->h);
	}
}

void	init_cluster_resources(t_game *game)
{
	int map_size = game->map->h * game->map->w;

	for (int i = 1; i < RESOURCES_NUMBER; i++) {

		int amount = game->aux->incantation_sum[i] * R_LEVEL;
		int cluster_size = (int)ceil((double)map_size /(double)amount);
		int curr_cell = 0;

		while (amount) {
			int temp_x = curr_cell % game->map->w;
			int temp_y = curr_cell / game->map->w;
			game->map->cells[temp_y][temp_x]->inventory[i]++;
			amount--;
			curr_cell = (curr_cell + cluster_size) % (game->map->w * game->map->h);
		}
	}
}

void	init_cluster_map(t_game *game)
{
	init_cluster_food(game);
	init_cluster_resources(game);
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