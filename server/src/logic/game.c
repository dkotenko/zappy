#include "zappy.h"
#include "../logger.h"
#include "../server.h"
#include "../reception.h"
#include "../utils.h"

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
		new[i]->id = i;
		new[i]->name = g_cfg.teams[i];
	}
	return new;
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
	game->hatchery = (t_hatchery *)ft_memalloc(sizeof(t_hatchery));
	return (game);
}


t_player *get_player_from_list_by_id(t_list *list, int player_id)
{
	while (list) {
		t_player *tmp_player = (t_player *)list->content;
		if (player_id == tmp_player->id) {
			return tmp_player;
		}
		list = list->next;
	}
	return NULL;
}

 /*
    The winning team is the one that will have its 6 players reach the maximum level.
     */
void	delete_player(t_player *player)
{
	t_list *visitor;
	t_player *player_in_teams;

	/* get visitor node from player's cell */
	visitor = ft_lstpop(&player->curr_cell->visitors, player);	
	xassert(visitor != NULL, "delete_player error: no player at cell");

	/* drop items */
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		player->curr_cell->inventory[i] += player->inventory[i];
	}
	if (!game->is_test) {
		bct_srv_event(player->curr_cell);
	}
	
	/* remove player from teams list */
	player_in_teams = get_player_from_list_by_id(game->teams[player->team_id]->players, player->id);
	if (player_in_teams) {
		game->teams[player->team_id]->players_num--;
		free(ft_lstpop(&game->teams[player->team_id]->players, player_in_teams));
		free(player_in_teams->inventory);
		free(player_in_teams);
	}

	/* remove real player*/
	game->players[player->id] = NULL;
	game->players_num--;
	free(player->inventory);
	free(player);
	free(visitor);
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

static t_player	*copy_player(t_player *player)
{
	t_player *copy = (t_player *)ft_memalloc(sizeof(t_player));
	ft_memcpy(copy, player, sizeof(t_player));
	copy->inventory = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	ft_memcpy(copy->inventory, player->inventory, sizeof(int) * RESOURCES_NUMBER);
	return copy;
}

t_player	*add_player(int player_id, int team_id)
{	
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
	t_player *copy = copy_player(player);
	ft_lstadd(&game->teams[player->team_id]->players, ft_lstnew_pointer(copy, sizeof(t_player)));
	return player;
}

t_list *add_visitor(t_cell *cell, t_player *player)
{
	t_list *new_player;

	new_player = ft_lstnew_pointer(player, sizeof(t_player));
	ft_lstadd(&cell->visitors, new_player);
	cell->visitors_num++;
	player->curr_cell = cell;
	log_debug("add_visitor: cell %d %d", cell->x, cell->y);
	return new_player;
}

void lgc_init(int max_players, int is_test)
{
	srand(time(NULL));
	game = create_game(init_cluster_map, is_test);
	game->players_size = max_players;
	game->players = (t_player **)ft_memalloc(sizeof(t_player *) * game->players_size);
	log_info("logic: World setup completed");
}

int	get_team_id(char *team_name/*, int *is_egg*/)
{
	for (int i = 0; i < g_cfg.teams_count; ++i)
	{
		if (strcmp(team_name, g_cfg.teams[i]) == 0)
			return i;
	}
/*
	t_list *hatchery_node = ft_lstfind(game->hatchery->eggs, team_name);
	if (hatchery_node) {
		*is_egg = 1;
		return ((t_egg *)hatchery_node->content)->team_id;
	}
*/
	return -1;
}


void delete_egg(t_egg *egg)
{
	free(ft_lstpop(&game->hatchery->eggs, egg));
	game->hatchery->eggs_num--;
	free(egg->token);
	free(egg);
}

/* it crashes, so using deletion by t_egg* */

/*
void delete_egg(char *token)
{
	t_list *hatchery_node = ft_lstfind(game->hatchery->eggs, token);
	t_egg *egg = (t_egg *)hatchery_node->content;
	ft_lstpop(&game->hatchery->eggs, egg);
	game->hatchery->eggs_num--;
	free(egg);
	free(hatchery_node);
}
*/

void lgc_new_player(int player_nb, char *team)
{
	int team_id = get_team_id(team);
	t_player *player = NULL;
	xassert(team_id != -1, "team does not exist");
	
	player = add_player(player_nb, team_id);

	/* Deleting egg moved to reception */

	/*
	if (is_egg) {
		delete_egg(team);
	}
	*/
	
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

void dump_visitors(int tick)
{
	const t_map *map = game->map;
	t_list *tmp;
	char filename[128] = "";
	char buf[512] = {0};
	char *p = buf;
	sprintf(filename, "dump_visitors/%d.dump.txt", tick);
	FILE *f = fopen(filename, "w");
	
	for (int i = 0; i < map->h; ++i) {
		for (int j = 0; j < map->w; ++j) {
			p = buf;
			tmp = map->cells[i][j]->visitors;
			if (!tmp)
				p += sprintf(p, "[ ]");
			while (tmp) {
				p += sprintf(p, "[%d]", ((t_player *)tmp->content)->id);
				tmp = tmp->next;
			}
			fprintf(f, "(%2d,%2d) %s ", i, j, buf);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void lgc_update(void)
{
	game->curr_tick++;
	/* dump_visitors(game->curr_tick); */
	if (is_session_end()) {
		end_game();
	} else {
		starving_n_death();
	}
}

void lgc_execute_command(int player_nb, char *cmd, int cmd_id)
{
	
	t_player *player = get_player_by_id(player_nb);
	if (cmd_id == -1) {
		cmd_id = lgc_get_command_id(cmd);
		if (cmd_id == -1) {
			log_warning("unknown command '%s'", cmd);
			return ;
		}
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
	for (int i = 0; i < CMD_NUMBER; ++i) {
		int len = strlen(g_cfg.cmd.name[i]);
		if (strncmp(cmd, g_cfg.cmd.name[i], len) == 0
				&& (cmd[len] == '\0' || cmd[len] == ' '))
			return i;
	}
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

