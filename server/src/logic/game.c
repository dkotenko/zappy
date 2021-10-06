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

void	add_player(int player_id)
{

}

void	add_visitor(t_cell *cell, t_player *player)
{
	t_list *new_player;

	new_player = ft_lstnew_pointer(player, sizeof(t_player));
	ft_lstadd(&cell->visitors, new_player);
	cell->visitors_num++;
}

t_player	*get_player_by_id(int player_id)
{
	return NULL;
	//return game->players[player_id];
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
		new->players = (t_player **)ft_memalloc(sizeof(t_player *) * g_cfg.max_clients_at_team);
	}
	return new;
}

t_game	*create_game()
{
	t_game	*game;

	game = (t_game *)ft_memalloc(sizeof(t_game));
	game->teams = create_teams();
    game->aux = create_aux();
	game->map = create_map(game, g_cfg.height, g_cfg.width);
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

t_player	*create_player(int player_id, int team_id)
{
	t_player	*player;

	player = (t_player *)ft_memalloc(sizeof(t_player));
	player->id = player_id;
	player->team_id = team_id;
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

int	get_team_id(char *team_name)
{
	for (int i = 0; i < g_cfg.teams_count; i++)
	{
		if (!strcmp(team_name, game->teams[i].name))
			return i;	
	}
	return -1;
}

void lgc_new_player(int player_nb, char *team)
{
	int	team_id = get_team_id(team);

	t_cell *cell = get_random_cell(game->map);
	t_player *player = create_player(player_nb, team_id);
	add_visitor(cell, player);
	//add_player(player);
	// add player to game

	log_info("logic: Add player #%d (team '%s')", player_nb, team);
	int x = player->curr_cell->x;
	int y = player->curr_cell->y;
	int o = player->orient;
	int l = player->level;
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
	if (is_session_ends()) {
		get_winners();
	}
	starving_n_death();
	// check all players, decrease hp, check win condition

}

void lgc_execute_command(int player_nb, char *cmd, int cmd_id)
{
	t_player *player = get_player_by_id(player_nb);
	if (cmd_id == -1) {
		cmd_id = lgc_get_command_id(cmd);
	}
	log_info("logic: Execute command '%s' from #%d", cmd, player_nb);

	if (g_cfg.cmd.req_arg[cmd_id]) {
		(g_cfg.cmd.f_arg[cmd_id])(player, cmd);
	} else {
		(g_cfg.cmd.f[cmd_id])(player);
	}
	/*


	

	if (strcmp(cmd, "avance") == 0) {
		avanche(player);
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "droite") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "gauche") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "voir") == 0) {
		srv_reply_client(player_nb, "{,,,}\n");
	} else if (strcmp(cmd, "inventaire") == 0) {
		srv_reply_client(player_nb, "{}\n");
	} else if (strcmp(cmd, "prend") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "pose") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "expulse") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "broadcast") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "incantation") == 0) {
		srv_reply_client(player_nb, "elevation en cours\n");
	} else if (strcmp(cmd, "fork") == 0) {
		srv_reply_client(player_nb, "ok\n");
	} else if (strcmp(cmd, "connect_nbr") == 0) {
		// TODO get team name from logic, not from reception (search is long)
		srv_reply_client(player_nb, "%d\n",
						 reception_slots_in_team(reception_find_client_team(player_nb)));
	} else {
		log_warning("Unknown command from player #%d: %s", player_nb, cmd);
	}

	// if event happens, call srv_event()
	*/
}

int lgc_get_command_id(char *cmd)
{
	if (strcmp(cmd, "avance") == 0)
		return CMD_AVANCE;
	if (strcmp(cmd, "droite") == 0)
		return CMD_DROITE;
	if (strcmp(cmd, "gauche") == 0)
		return CMD_GAUCHE;
	if (strcmp(cmd, "voir") == 0)
		return CMD_VOIR;
	if (strcmp(cmd, "inventaire") == 0)
		return CMD_INVENTAIRE;
	if (strncmp(cmd, "prend ", 6) == 0)
		return CMD_PREND;
	if (strncmp(cmd, "pose ", 5) == 0)
		return CMD_POSE;
	if (strcmp(cmd, "expulse") == 0)
		return CMD_EXPULSE;
	if (strncmp(cmd, "broadcast ", 10) == 0)
		return CMD_BROADCAST;
	if (strcmp(cmd, "incantation") == 0)
		return CMD_INCANTATION;
	if (strcmp(cmd, "fork") == 0)
		return CMD_FORK;
	if (strcmp(cmd, "connect_nbr") == 0)
		return CMD_CONNECT_NBR;
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
	t_player *player = get_player_by_id(player_nb);

	if (!player)
		return -1;
	*x = player->curr_cell->x;
	*y = player->curr_cell->y;
	*o = player->orient;
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
