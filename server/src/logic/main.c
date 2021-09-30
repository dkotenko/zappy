#include "zappy.h"
#include "../logger.h"
#include "../server.h"
#include "../reception.h"

int is_session_ends()
{
    return (0);
}



void	add_visitor(t_cell *cell, t_client *client)
{
	t_list *new_client;

	new_client = ft_lstnew_pointer(client, sizeof(t_client));
	ft_lstadd(&cell->visitors, new_client);
	
	
	
	cell->visitors_num++;
}


//, int (*functionPtr)(int, int)





int print_client(struct s_cell *cell)
{
	if (cell->visitors_num)
		return (1);
	return (0);
}

t_client	*create_client(t_game *game, t_cell *cell)
{
	t_client	*client;

	client = (t_client *)ft_memalloc(sizeof(t_client));

	client->curr_cell = cell;
	client->id = game->clients_num++;
	
	client->hp = INIT_HP;
	client->orient = rand() % 4;
	client->inventory = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	client->curr_cell = cell;
	client->buf = t_buffer_create(0);
	client->is_egg = 0;
	return (client);
}



t_game	*create_game()
{
	t_game	*game;

	game = (t_game *)ft_memalloc(sizeof(t_game));
    game->aux = create_aux();
	return (game);
}



int main_old()
{
	t_game *game;
	
	srand(time(NULL));
	int h = 10;
	int w = 10;
	
	
	//game is global 
	game = create_game();
	game->clients_num = 2;
	game->teams_num = 2;
	
	game->map = create_map(game, w, h);
	
	exit(0);
	//t_cell *temp = get_random_cell(map);
	t_cell *temp = game->map->cells[1][1];
	t_client *client = create_client(game, temp); 
	
	add_visitor(temp, client);
	print_map(game, print_client);
	printf("%d before\n", client->orient);
	gauche(client, game->aux);
	printf("%d after\n", client->orient);
	print_map(game, print_client);
	

	 /*
    The winning team is the one that will have its 6 players reach the maximum level.
     */

    
	exit(0);
	return (0);
}

t_game *game;

void lgc_init(void)
{
	log_info("logic: Setup world");
	srand(time(NULL));
	game = create_game();
	game->teams_num = g_main_config.teams_count;
	game->map = create_map(game, g_main_config.world_width, g_main_config.world_height);
}


void lgc_new_player(int client_nb, char *team)
{
	t_cell *temp = game->map->cells[1][1];
	t_client *client = create_client(game, temp); 
	
	add_visitor(temp, client);
	// add client to game

	log_info("logic: Add player #%d (team '%s')", client_nb, team);
	int x = 0;
	int y = 0;
	int o = 2;
	int l = 1;
	srv_event("pnw %d %d %d %d %d %s\n", client_nb, x, y, o, l, team);
}

void lgc_player_gone(int client_nb)
{
	// delete player from game
	log_info("logic: Remove client #%d", client_nb);
	srv_event("pdi %d\n", client_nb);
}

void lgc_update(void)
{

	// check all players, decrease hp, check win condition

}


int lgc_get_command_duration(char *cmd)
{
	if (strcmp(cmd, "avance") == 0)
		return 7;
	if (strcmp(cmd, "droite") == 0)
		return 7;
	if (strcmp(cmd, "gauche") == 0)
		return 7;
	if (strcmp(cmd, "voir") == 0)
		return 7;
	if (strcmp(cmd, "inventaire") == 0)
		return 1;
	if (strcmp(cmd, "prend") == 0)
		return 7;
	if (strcmp(cmd, "pose") == 0)
		return 7;
	if (strcmp(cmd, "expulse") == 0)
		return 7;
	if (strcmp(cmd, "broadcast") == 0)
		return 7;
	if (strcmp(cmd, "incantation") == 0)
		return 300;
	if (strcmp(cmd, "fork") == 0)
		return 42;
	if (strcmp(cmd, "connect_nbr") == 0)
		return 0;
	return -1;
}

void lgc_execute_command(int client_nb, char *cmd)
{
	if (strcmp(cmd, "connect_nbr") == 0) {
		// TODO get team name from logic, not from reception (search is long)
		srv_reply_client(client_nb, "%d\n",
						 reception_slots_in_team(reception_find_client_team(client_nb)));
	} else {
		log_info("logic: Execute command '%s' from #%d", cmd, client_nb);
		srv_reply_client(client_nb, "%s - ok\n", cmd);
	}


	// if event happens, call srv_event()
}

int lgc_get_cell_resources(int x, int y, int resources[7])
{
	if (x < 0 || y < 0)
		return -1;
	for (int i = 0; i < 7; ++i)
		resources[i] = i;
	return 1;
}

int lgc_get_player_position(int client_nb, int *x, int *y, int *o)
{
	if (client_nb < 0)
		return -1;
	*x = 12;
	*y = 34;
	*o = 2;
	return 0;
}

int lgc_get_player_level(int client_nb)
{
	if (client_nb < 0)
		return -1;
	return 1;
}

int lgc_get_player_inventory(int client_nb, int *x, int *y, int resources[7])
{
	if (client_nb < 0)
		return -1;
	*x = 34;
	*y = 56;

	for (int i = 0; i < 7; ++i)
		resources[i] = i + 1;
	return 0;
}


// TODO
// lgc_get_all_players(...)

// Нужно передать о всех клиентах следующую инфу: координаты, ориентация, уровень, название команды
// например в виде массива структур
// нужно в визуализаторе при его подключении
