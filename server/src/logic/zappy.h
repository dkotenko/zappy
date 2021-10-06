#ifndef ZAPPY_H
# define ZAPPY_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include "libft.h"
# include "color.h"
# include "const.h"
# include "../logic.h"

typedef struct s_aux
{
    int     *orientation;
	int		*orient_chars;
    char    **resources;
    int     *len_resources;
	int		**incantation;
	int		*incantation_sum;
}   t_aux;

typedef struct s_action {
    void *f;
    char *name;
    double delai;
    char *response;
} t_action;

typedef struct s_inventory {
    int nourriture;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} t_inventory;

typedef struct s_resource {
    int num;
    char *name;
} t_resource;

/*
 * клетка карты
 */
typedef struct s_cell {
    int x;
    int y;
    int *inventory;
    t_list *visitors;
    int visitors_num;
}		t_cell;

typedef struct s_player
{
    int     id;
    int     level;
    int     hp;
    int     orient;
    int     *inventory;
    t_cell  *curr_cell;
    int     is_egg;
    int     team_id;
}	t_player;

typedef struct s_team
{
	int id;
    char *name;
    t_player **players;
} t_team;

typedef struct s_config {
    int port;
    int w;
    int h;
    char **teams;
    int timeDivider; // 1;
    int moniteur_number; // 1;
    int team_number;
    t_buffer *buf;
    //t_player *players;
} t_config;

typedef struct s_map
{
    t_cell  ***cells;
    int     w;
    int     h;
} t_map;

typedef struct s_game
{
    t_aux   *aux;
    t_team  *teams;
    int      teams_num;
	t_list	*players;
	int		players_num;
	t_map	*map;
    t_buffer *buf;
} t_game;


typedef struct s_msg
{
    char        *message;
    int         socket;
    long long   timestamp;
}   t_msg;

typedef struct s_token
{
    char    *token;
    int     team_id;
} t_token;




t_aux   *create_aux();
void	init_map(t_game *game);
t_map	*create_map(t_game *game, int w, int h);
void print_map(t_game *game, int (*f)(t_cell *));

/*
 * json
 */
void	t_buffer_json_key(t_buffer *buf, char *key);
void	t_buffer_json_message(t_buffer *buf, char *message);
void	write_cell_json(t_cell *cell);
void	t_buffer_json_message_all(t_list *players, t_buffer *buf, char *message, t_player *player);

/*
 *	random
 */ 
t_cell	*get_random_cell(t_map *map);
int		get_random_in_range(int max_number);
int     get_random_from_to(int min_num, int max_num);


void	inventory(int *inv);
#endif
