#include "zappy.h"
#include "../logger.h"
#include "../server.h"
#include "../reception.h"
#include "../utils.h"

t_game *game;

int is_session_ends()
{
    return (0);
}



void	add_visitor(t_cell *cell, t_player *player)
{
	t_list *new_player;

	new_player = ft_lstnew_pointer(player, sizeof(t_player));
	ft_lstadd(&cell->visitors, new_player);
	
	
	
	cell->visitors_num++;
}

//, int (*functionPtr)(int, int)

int print_player(struct s_cell *cell)
{
	if (cell->visitors_num)
		return (1);
	return (0);
}

t_team	*create_teams()
{
	t_team	*new;

	new = (t_team *)ft_memalloc(sizeof(t_team) * g_cfg.teams_count);
	for (int i = 0; i < g_cfg.teams_count; i++) {
		new[i].id = i + 1;
		new->name = g_cfg.teams[i];
		new->players = (t_player *)ft_memalloc(sizeof(t_player) * g_cfg.max_clients_at_team);
	}
	return new;
}

t_game	*create_game()
{
	t_game	*game;

	game = (t_game *)ft_memalloc(sizeof(t_game));
	game->teams_num = g_cfg.teams_count;
	game->teams = create_teams();
    game->aux = create_aux();
	game->map = create_map(game, g_cfg.world_height, g_cfg.world_width);
	game->buf = t_buffer_create(0);
	return (game);
}

/*
int main_old()
{
	t_game *game;
	
	srand(time(NULL));
	int h = 10;
	int w = 10;
	
	//game is global 
	game = create_game();
	game->players_num = 2;
	game->teams_num = 2;
	
	
	
	exit(0);
	//t_cell *temp = get_random_cell(map);
	t_cell *temp = game->map->cells[1][1];
	t_player *player = create_player(1); 
	
	add_visitor(temp, player);
	print_map(game, print_player);
	printf("%d before\n", player->orient);
	gauche(player, game->aux);
	printf("%d after\n", player->orient);
	print_map(game, print_player);

	exit(0);
	return (0);
}
*/

 /*
    The winning team is the one that will have its 6 players reach the maximum level.
     */


void lgc_init(void)
{
	log_info("logic: Setup world");
	srand(time(NULL));
	game = create_game();
	
}

/*
void	delete_player(t_player *player)
{
	
}
*/

t_player	*create_player(int player_id)
{
	t_player	*player;

	player = (t_player *)ft_memalloc(sizeof(t_player));
	player->id = player_id;
	game->players_num++;
	player->hp = INIT_HP;
	player->orient = rand() % 4;
	player->inventory = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	player->is_egg = 0;
	return (player);
}

/*
void	player_set_cell(t_player *player, t_cell *cell)
{

}
*/

void lgc_new_player(int player_nb, char *team)
{
	t_cell *cell = get_random_cell(game->map);
	t_player *player = create_player(player_nb); 
	add_visitor(cell, player);
	//add_player(player);
	// add player to game

	log_info("logic: Add player #%d (team '%s')", player_nb, team);
	int x = 0;
	int y = 0;
	int o = 2;
	int l = 1;
	srv_event("pnw %d %d %d %d %d %s\n", player_nb, x, y, o, l, team);
}

void lgc_player_gone(int player_nb)
{
	// delete player from game
	log_info("logic: Remove player #%d", player_nb);
	srv_event("pdi %d\n", player_nb);
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


void lgc_execute_command(int player_nb, char *cmd)
{
	log_info("logic: Execute command '%s' from #%d", cmd, player_nb);
	t_player *player = get_player_by_id(player_nb);

	if (strcmp(cmd, "avance") == 0) {
		avanche(player);
	}
	if (strcmp(cmd, "droite") == 0) {

	}
		
	if (strcmp(cmd, "gauche") == 0)
		
	if (strcmp(cmd, "voir") == 0)
		
	if (strcmp(cmd, "inventaire") == 0)
		
	if (strcmp(cmd, "prend") == 0)
		
	if (strcmp(cmd, "pose") == 0)
		
	if (strcmp(cmd, "expulse") == 0)
		
	if (strcmp(cmd, "broadcast") == 0)
		
	if (strcmp(cmd, "incantation") == 0)
		
	if (strcmp(cmd, "fork") == 0)
		
	if (strcmp(cmd, "connect_nbr") == 0) {

	} else if (strcmp(cmd, "connect_nbr") == 0) {
		// TODO get team name from logic, not from reception (search is long)
		srv_reply_player(player_nb, "%d\n",
						 reception_slots_in_team(reception_find_player_team(player_nb)));
	} else {
		
		//srv_reply_player(player_nb, "%s - ok\n", cmd);
	}


	// if event happens, call srv_event()
}

int lgc_get_command_id(char *cmd)
{
	if (strncmp(cmd, "avance", 6) == 0)
		CMD_AVANCE;
	if (strncmp(cmd, "droite", 6) == 0)
		CMD_DROITE;
	if (strncmp(cmd, "gauche", 6) == 0)
		CMD_GAUCHE;
	if (strncmp(cmd, "voir", 4) == 0)
		CMD_VOIR;
	if (strncmp(cmd, "inventaire", 10) == 0)
		CMD_INVENTAIRE;
	if (strncmp(cmd, "prend", 5) == 0)
		CMD_PREND;
	if (strncmp(cmd, "pose", 4) == 0)
		CMD_POSE;
	if (strncmp(cmd, "expulse", 7) == 0)
		CMD_EXPULSE;
	if (strncmp(cmd, "broadcast", 9) == 0)
		CMD_BROADCAST;
	if (strncmp(cmd, "incantation", 9) == 0)
		CMD_INCANTATION;
	if (strncmp(cmd, "fork", 4) == 0)
		CMD_FORK;
	if (strncmp(cmd, "connect_nbr", 11) == 0)
		CMD_CONNECT_NBR;
	return -1;
}


int lgc_get_cell_resources(int x, int y, int resources[7])
{
	if (x < 0 || y < 0)
		return -1;
	for (int i = 0; i < 7; ++i)
		resources[i] = i;
	return 1;
}

int lgc_get_player_position(int player_nb, int *x, int *y, int *o)
{
	if (player_nb < 0)
		return -1;
	*x = 12;
	*y = 34;
	*o = 2;
	return 0;
}

int lgc_get_player_level(int player_nb)
{
	if (player_nb < 0)
		return -1;
	return 1;
}

int lgc_get_player_inventory(int player_nb, int *x, int *y, int resources[7])
{
	if (player_nb < 0)
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
