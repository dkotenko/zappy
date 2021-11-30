#include "zappy.h"
#include "../logger.h"
#include "../server.h"
#include "../reception.h"
#include "../utils.h"

#define MAX_FD_MACOS 12288

t_game *game;

void	mock_srv_reply(int client_nb, char *msg)
{
	printf("\n%sClient %d received message: %s%s\n\n",
		ANSI_COLOR_YELLOW, client_nb, msg, ANSI_COLOR_RESET);
}

void	reply_except_list(t_list *list, int player_id)
{
	while (list)
	{
		t_player *player = (t_player *)list->content;
		if (player->id != player_id) {
			srv_reply_client(player->id, "%s\n", game->buf->s);	
		}
		list = list->next;
	}
	t_buffer_clean(game->buf);
}

void	reply_client(int player_id)
{
	if (game->is_test == 0) {
		srv_reply_client(player_id, "%s\n", game->buf->s);
	} else {
		mock_srv_reply(player_id, game->buf->s);
	}
}

void	reply_and_clean_buff(int player_id)
{
	reply_client(player_id);
	t_buffer_clean(game->buf);
}

int is_session_ends()
{
    return (0);
}


t_player	*get_player_by_id(int player_id)
{
	return game->players[player_id];
}

t_team	**create_teams()
{
	t_team	**new;

	new = (t_team **)ft_memalloc(sizeof(t_team *) * g_cfg.teams_count);
	for (int i = 0; i < g_cfg.teams_count; i++) {
		new[i] = ft_memalloc(sizeof(t_team));
		new[i]->id = i + 1;
		new[i]->name = g_cfg.teams[i];
		new[i]->players = (t_player **)ft_memalloc(sizeof(t_player *) * g_cfg.max_clients_at_team);
	}
	return new;
}

int	get_maxfd()
{
	return MAX_FD_MACOS;
	srv_get_maxfd();
}

t_game	*create_game(map_initiator init_map, int is_test)
{
	t_game	*game;

	game = (t_game *)ft_memalloc(sizeof(t_game));
	game->is_test = is_test;
	game->teams_num = g_cfg.teams_count;
	game->teams = create_teams();
    game->aux = create_aux();
	game->map = create_map(game, g_cfg.width, g_cfg.height);
	(*init_map)(game);
	game->buf = t_buffer_create(0);
	/*
	** game->players initialization moved to add_player due to get_maxfd issue
	*/
	game->hatchery = (t_hatchery *)ft_memalloc(sizeof(t_hatchery));
	return (game);
}

int get_players_num()
{
	return 0;
}


 /*
    The winning team is the one that will have its 6 players reach the maximum level.
     */
void	delete_player(t_player *player)
{
	t_list *tmp = player->curr_cell->visitors;
	t_player *tmp_player = NULL;
	
	while (tmp) {
		tmp_player = (t_player *)tmp->content;
		if (player == tmp_player) {
			break ;
		}
		tmp = tmp->next;
	}
	xassert(tmp_player != NULL, "delete_player error: no player at cell");
	tmp = ft_lstpop(&player->curr_cell->visitors, tmp_player);	
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		tmp_player->curr_cell->inventory[i] += tmp_player->inventory[i];
	}

	game->players[player->id] = NULL;
	game->players_num--;
	game->teams[player->team_id]->players_num--;
	free(player->inventory);
	free(player);
	free(tmp);

	if (game->is_test)
		return ;
	bct_srv_event(tmp_player->curr_cell);
	
}


t_player	*create_player(int player_id, int team_id)
{
	t_player	*player;

	player = (t_player *)ft_memalloc(sizeof(t_player));
	player->id = player_id;
	player->team_id = team_id;
	player->orient = rand() % 4 + 1;
	player->inventory = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	player->inventory[0] += 10;
	player->level = 1;
	return (player);
}

void		add_player_egg(int hatchery_id)
{
	
}

t_player	*add_player(int player_id, int team_id)
{
	if (!game->players) {
		game->players = (t_player **)ft_memalloc(sizeof(t_player *) * get_maxfd());
	}
	
	int is_error = xassert(game->players[player_id] == NULL, "add_player: player already exists");
	
	if (is_error) {
		char *player_id_str = ft_itoa(player_id);
		xassert(0, "player with id: ");
		xassert(0, player_id_str);
		free(player_id_str);
		return NULL;
	}	
	
	t_cell *cell = get_random_cell(game->map);
	t_player *player = create_player(player_id, team_id);

	player->last_meal_tick = game->curr_tick;
	add_visitor(cell, player);
	game->players[player->id] = player;
	game->players_num++;
	game->teams[player->team_id]->players_num++;
	
	return player;
}

void	

add_visitor(t_cell *cell, t_player *player)
{
	t_list *new_player;

	new_player = ft_lstnew_pointer(player, sizeof(t_player));
	ft_lstadd(&cell->visitors, new_player);
	cell->visitors_num++;
	player->curr_cell = cell;
	log_debug("add_visitor: cell %d %d", cell->x, cell->y);
}

void lgc_init(int is_test)
{
	srand(time(NULL));
	game = create_game(init_cluster_map, is_test);
	log_info("logic: World setup completed");
}

int	get_team_id(char *team_name)
{
	for (int i = 0; i < g_cfg.teams_count; ++i)
	{
		if (strcmp(team_name, g_cfg.teams[i]) == 0)
			return i;
	}
	return -1;
}

void lgc_new_player(int player_nb, char *team)
{
	int team_id = get_team_id(team);
	t_player *player =  add_player(player_nb, team_id);
	log_info("logic: Add player #%d (team '%s')", player_nb, team);

	//SEND TO MONITEUR
	int x = player->curr_cell->x;
	int y = player->curr_cell->y;
	int o = player->orient;
	int l = player->level;
	srv_event("pnw %d %d %d %d %d %s\n", player_nb, x, y, o, l, team);
}

void lgc_player_gone(int player_nb)
{
	log_info("logic: Remove player #%d", player_nb);
	srv_event("pdi %d\n", player_nb);
	srv_client_died(player_nb);
	delete_player(get_player_by_id(player_nb));
}

void lgc_update(void)
{
	game->curr_tick++;
	if (is_session_ends()) {
		get_winners();
	} else {
		starving_n_death();
	}
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
	if (player_nb)
		reply_and_clean_buff(player_nb);
	/*
	if (strcmp(cmd, "connect_nbr") == 0) {
		// TODO get team name from logic, not from reception (search is long)
		srv_reply_client(player_nb, "%d\n",
						 reception_slots_in_team(reception_find_client_team(player_nb)));
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
	if (strncmp(cmd, "prend", 5) == 0)
		return CMD_PREND;
	if (strncmp(cmd, "pose", 4) == 0)
		return CMD_POSE;
	if (strcmp(cmd, "expulse") == 0)
		return CMD_EXPULSE;
	if (strncmp(cmd, "broadcast", 9) == 0)
		return CMD_BROADCAST;
	if (strcmp(cmd, "incantation") == 0)
		return CMD_INCANTATION;
	if (strcmp(cmd, "incantation_end") == 0)
		return CMD_INCANTATION_END;
	if (strcmp(cmd, "fork") == 0)
		return CMD_FORK;
	if (strcmp(cmd, "connect_nbr") == 0)
		return CMD_CONNECT_NBR;
	if (strncmp(cmd, "restore_resource", 16) == 0)
		return CMD_RESTORE_RESOURCE;
	return -1;
}

int lgc_get_cell_resources(int x, int y, int resources[7])
{
	if (x < 0 || y < 0)
		return -1;
	for (int i = 0; i < 7; ++i)
		resources[i] = game->map->cells[y][x]->inventory[i];
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
	t_player *player = get_player_by_id(player_nb);

	if (!player)
		return -1;
	return player->level;
}

int lgc_get_player_inventory(int player_nb, int *x, int *y, int resources[7])
{
	t_player *player = get_player_by_id(player_nb);
	if (!player)
		return -1;

	*x = player->curr_cell->x;
	*y = player->curr_cell->y;

	for (int i = 0; i < 7; ++i)
		resources[i] = player->inventory[i];
	return 0;
}

