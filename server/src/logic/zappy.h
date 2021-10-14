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

typedef struct s_inventory {
    int nourriture;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} t_inventory;

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
    int     orient;
	int     is_egg;
    int     team_id;
    int     *inventory;
    t_cell  *curr_cell;
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
	t_player	**players;
    int      teams_num;
	int		players_num;
	t_map	*map;
    t_buffer *buf;
	int		is_test;
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

typedef void (*map_initiator)(t_game *);
void	init_random_map(t_game *game);
void	init_logic_map(t_game *game);
void	mock_init_map(t_game *game);
void	clear_resources_map(t_game *game);
/*
 * game
 */

t_aux   *create_aux();
void	init_map(t_game *game);
t_map	*create_map(t_game *game, int w, int h);

t_player	*create_player(int player_id, int team_id);
void	reply_and_clean_buff(int player_id);
void	mock_srv_reply(int client_nb, char *msg);

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

void	add_visitor(t_cell *cell, t_player *player);
t_player	*add_player(int player_id, int team_id);
void	move_player_to_cell(t_player *player, t_cell *cell);

/*
** print
*/
void	print_player(t_player *player);
void	print_cell(t_cell *cell);
void	print_visitors(t_list *visitors);
void	print_inverntory(int *inv);
void 	print_map(t_game *game, int (*f)(t_cell *));

#endif
