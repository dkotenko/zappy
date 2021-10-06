#include "zappy.h"
#include "../server.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))


extern t_game *game;

void	get_forward_coords(t_player *player, int *new_x, int *new_y);

int		get_x(int coord)
{
	if (coord < 1)
		coord += config.w;
	else if (coord > config.w)
		coord -= config.w;
	return coord;
}

int		get_y(int coord)
{
	if (coord < 1)
		coord += config.h;
	else if (coord > config.h)
		coord -= config.h;
	return coord;
}


void	mort(t_player *player)
{
	t_list *tmp_list = ft_listpop(game->players, tmp);
	tmp_player = (t_player *)tmp_list->content;
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		tmp_player->curr_cell->inventory[i] += tmp_player->inventory[i];
	}
	free(tmp_player->inventory);
	free(tmp_player);
	free(tmp_list);
}

//NOT TESTED
void	starving_n_death(t_game *game)
{
	t_list		*tmp;
	t_player	*tmp_player;

	tmp = game->players;
	while (tmp) {
		tmp_player = (t_player *)game->players->content;
		if (!tmp_player->hp) {
			mort(tmp_player);
		}
		tmp_player->hp--;
		tmp = tmp->next;
	}
}



void	avanche(t_player *player)
{
	int	new_x = 0;
	int new_y = 0;

	get_forward_coords(player, &new_x, &new_y);
	t_list *temp = ft_listpop(player->curr_cell->visitors, player);
	game->map->cells[new_y][new_x]->visitors->next = temp;
	t_buffer_json_message(config.buf, "OK");
}


//TESTED
//returns OK
void	droite(t_player *player)
{
	
	player->orient = game->aux->orientation[(player->orient + 1) % 4];

	t_buffer_json_message(config.buf, "OK");
}


//TESTED
//returns OK
void	gauche(t_player *player)
{
	player->orient = game->aux->orientation[(player->orient + 4 - 1) % 4];
	t_buffer_json_message(config.buf, "OK");
}


//returns items
void	inventory(int *inv)
{
	t_buffer_write(config.buf, "{ ");
	t_buffer_json_key(config.buf, "inventory");
	t_buffer_write(config.buf, "{ ");
	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		t_buffer_json_key(config.buf, game->aux->resources[i]);
		t_buffer_write_int(config.buf, inv[i]);
		if (i != RESOURCES_NUMBER - 1) {
			t_buffer_write(config.buf, ", ");
		}
	}
	t_buffer_write(config.buf, " }}");
}




int		get_w(int coord)
{
	if (coord < 0)
		coord += config.w;
	else if (coord >= config.w)
		coord -= config.w;
	return coord;
}

int		get_h(int coord)
{
	if (coord < 0)
		coord += config.h;
	else if (coord >= config.h)
		coord -= config.h;
	return coord;
}






//returns cells
void	voir(t_player *player)
{	
	t_map *map = game->map;

	int printed[map->h][map->w];
	memset(printed, 0, sizeof(printed));
	int x = player->curr_cell->x;
	int y = player->curr_cell->y;
	
	int orient = player->orient;
	if (orient == ORIENT_N || orient == ORIENT_S) {
		for (int i = 0; i < player->level + 1; i++) {
			int h = get_h(y + (orient == ORIENT_S ? i : -i));
			int w = max(x - i, 0);
			int max_w = min(x + 1 + i, map->w);
			while (w < max_w) {
				printed[h][get_w(w)] = 1;
				w++;
			}
		}
	} else if (orient == ORIENT_W || orient == ORIENT_E) {
		for (int i = 0; i < player->level + 1; i++) {
			int w = get_w(x + (orient == ORIENT_W ? -i : i));
			int h = max(y - i, 0);
			int max_h = min(y + 1 + i, map->h);
			while (h < max_h) {
				printed[get_h(h)][w] = 1;
				h++;
			}
		}
	}
	else {
		handle_error("INVALID ORIENTATION\n");
	}
	
	
	t_buffer_write(config.buf, "{ ");
	t_buffer_json_key(config.buf, "cells");
	t_buffer_add_char(config.buf, '[');
	for (int i = 0; i < map->h; i++) {
		for (int j = 0; j < map->w; j++) {
			if (printed[i][j]) {
				write_cell_json(map->cells[i][j]);
			}
		}
	}
	t_buffer_write(config.buf, "]}");
}

void	get_forward_coords(t_player *player, int *new_x, int *new_y)
{
	int x = player->curr_cell->x;
	int y = player->curr_cell->y;

	if (player->orient == ORIENT_E)
	{
		*new_x = get_x(player->curr_cell->x + 1);
		*new_y = y;
	}
	else if (player->orient == ORIENT_N)
	{
		*new_x = x;
		*new_y = get_y(player->curr_cell->y + 1);
	}
	else if (player->orient == ORIENT_W)
	{
		*new_x = get_x(player->curr_cell->x - 1);
		*new_y = y;
	}
	else if (player->orient == ORIENT_S)
	{
		*new_x = x;
		*new_y = get_y(player->curr_cell->y - 1);
	}
	else {
		handle_error("expulse error: invalid orient");
	}
}

//return ok//ko
void	expulse(t_player *player)
{
	int	new_x = 0;
	int new_y = 0;
	
	get_forward_coords(player, &new_x, &new_y);
	t_list *temp = ft_listpop(player->curr_cell->visitors, player);
	if (player->curr_cell->visitors) {
		game->map->cells[new_y][new_x]->visitors->next = player->curr_cell->visitors;
		t_buffer_json_message(config.buf, "OK");
		t_buffer_json_message_all(player->curr_cell->visitors, config.buf, "deplacement", player);
	} else {
		t_buffer_json_message(config.buf, "KO");
	}
	player->curr_cell->visitors = temp;
}


static int	get_resource_id(t_aux *aux, char *resource)
{
	int	i;

	i = -1;
	while (++i < 7)
	{
		if (!strcmp(resource, aux->resources[i]))
			return i;
	}
	return (-1);
}

//NOT TESTED
void	prend(t_player *player, char *data)
{
	char	*resource = data + strlen("prend ");
	int	resource_id = atoi(resource);
	
	if (!resource_id && resource[0] != '0' || resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		t_buffer_json_message(config.buf, "KO");
	} else if (player->curr_cell->inventory[resource_id] > 0)
	{
		player->curr_cell->inventory[resource_id]--;
		player->inventory[resource_id]++;
		t_buffer_json_message(config.buf, "OK");
	} else {
		t_buffer_json_message(config.buf, "KO");
	}
}

static char *get_resource(char *data)
{
	
	return (NULL);
}

//NOT TESTED
void	pose(t_player *player, char *data)
{
	char	*resource = data + strlen("pose ");
	int	resource_id = atoi(resource);

	if (!resource_id && resource[0] != '0' || resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		t_buffer_json_message(config.buf, "KO");
	} else if (player->inventory[resource_id] > 0) {
		player->inventory[resource_id]--;
		player->curr_cell->inventory[resource_id]++;
		t_buffer_json_message(config.buf, "OK");
	} else {
		t_buffer_json_message(config.buf, "KO");
	}
}


void	broadcast(t_player *player, char *data)
{
	char	*text = data + strlen("broadcast ");
}


//NOT TESTED
void	incantation(t_game *game, t_player *player)
{
	t_list *temp = player->curr_cell->visitors;
	int	*incat_consts = game->aux->incantation[player->level - 1];

	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		if (player->inventory[i] < incat_consts[i]) {
			t_buffer_json_message(config.buf, "KO");
			return ;	
		}
	}

	int	incat_counter = incat_consts[RESOURCES_NUMBER_OF_PLAYERS] - 1;
	t_player *participants[incat_counter];
	while (temp && incat_counter) {
		t_player *temp_player = (t_player *)temp->content;
		if (temp_player != player && temp_player->level == player->level) {
			participants[--incat_counter] = temp_player;
		}
		temp = temp->next;
	}
	if (incat_counter) {
		t_buffer_json_message(config.buf, "KO");
		return ;
	}

	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		player->inventory[i] -= incat_consts[i];
	}

	for (int i = 0; i < incat_consts[RESOURCES_NUMBER_OF_PLAYERS] - 1; i++) {
		participants[i]->level++;
	}
	player->level++;
}

// 8-symbol token, symbols in range [A : Z]
t_token *create_token(int team_id)
{
	t_token	*new = (t_token *)ft_memalloc(sizeof(t_token));
	new->team_id = team_id;
	new->token = ft_strnew(9);
	for (int i = 0; i < 8; i++) {
		new->token[i] = get_random_from_to(65, 91);
	}
	return (new);
}


void	do_fork(t_player *player)
{
	//add_player(create_player);
	player->is_egg = 1;
	//t_buf write OK, write token
}


void	connect_nbr(t_player *player)
{
	player = NULL;
}



void	init_cmd()
{
	g_cfg.cmd.duration = (int *)malloc(sizeof(int) * CMD_NUMBER);
	xassert(g_cfg.cmd.duration != NULL, "malloc");

	g_cfg.cmd.name = (char **)malloc(sizeof(char *) * CMD_NUMBER);
	xassert(g_cfg.cmd.name != NULL, "malloc");

	g_cfg.cmd.f_arg = (action_arg *)malloc(sizeof(action_arg *) * CMD_NUMBER);
	xassert(g_cfg.cmd.f != NULL, "malloc");

	g_cfg.cmd.f = (action *)malloc(sizeof(action *) * CMD_NUMBER);
	xassert(g_cfg.cmd.f != NULL, "malloc");

	g_cfg.cmd.req_arg = (int *)malloc(sizeof(int) * CMD_NUMBER);
	xassert(g_cfg.cmd.req_arg != NULL, "malloc");

	g_cfg.cmd.duration[CMD_AVANCE] = 7;
	g_cfg.cmd.duration[CMD_DROITE] = 7;
	g_cfg.cmd.duration[CMD_GAUCHE] = 7;
	g_cfg.cmd.duration[CMD_VOIR] = 7;
	g_cfg.cmd.duration[CMD_INVENTAIRE] = 1;
	g_cfg.cmd.duration[CMD_PREND] = 7;
	g_cfg.cmd.duration[CMD_POSE] = 7;
	g_cfg.cmd.duration[CMD_EXPULSE] = 7;
	g_cfg.cmd.duration[CMD_BROADCAST] = 7;
	g_cfg.cmd.duration[CMD_INCANTATION] = 300;
	g_cfg.cmd.duration[CMD_FORK] = 42;
	g_cfg.cmd.duration[CMD_CONNECT_NBR] = 0;

	g_cfg.cmd.name[CMD_AVANCE] = strdup("avance");
	g_cfg.cmd.name[CMD_DROITE] = strdup("droite");
	g_cfg.cmd.name[CMD_GAUCHE] = strdup("gauche");
	g_cfg.cmd.name[CMD_VOIR] = strdup("voir");
	g_cfg.cmd.name[CMD_INVENTAIRE] = strdup("inventaire");
	g_cfg.cmd.name[CMD_PREND] = strdup("prend");
	g_cfg.cmd.name[CMD_POSE] = strdup("pose");
	g_cfg.cmd.name[CMD_EXPULSE] = strdup("expulse");
	g_cfg.cmd.name[CMD_BROADCAST] = strdup("broadcast");
	g_cfg.cmd.name[CMD_INCANTATION] = strdup("incantation");
	g_cfg.cmd.name[CMD_FORK] = strdup("fork");
	g_cfg.cmd.name[CMD_CONNECT_NBR] = strdup("connect_nbr");

	g_cfg.cmd.f[CMD_AVANCE] = avanche;
	g_cfg.cmd.f[CMD_DROITE] = droite;
	g_cfg.cmd.f[CMD_GAUCHE] = gauche;
	g_cfg.cmd.f[CMD_VOIR] = voir;
	g_cfg.cmd.f[CMD_INVENTAIRE] = inventory;
	g_cfg.cmd.f[CMD_EXPULSE] = expulse;
	g_cfg.cmd.f[CMD_INCANTATION] = incantation;
	g_cfg.cmd.f[CMD_FORK] = do_fork;
	g_cfg.cmd.f[CMD_CONNECT_NBR] = connect_nbr;

	g_cfg.cmd.f_arg[CMD_PREND] = prend;
	g_cfg.cmd.f_arg[CMD_POSE] = pose;
	g_cfg.cmd.f_arg[CMD_BROADCAST] = broadcast;
	
	g_cfg.cmd.req_arg[CMD_AVANCE] = 0;
	g_cfg.cmd.req_arg[CMD_DROITE] = 0;
	g_cfg.cmd.req_arg[CMD_GAUCHE] = 0;
	g_cfg.cmd.req_arg[CMD_VOIR] = 0;
	g_cfg.cmd.req_arg[CMD_INVENTAIRE] = 0;
	g_cfg.cmd.req_arg[CMD_PREND] = 1;
	g_cfg.cmd.req_arg[CMD_POSE] = 1;
	g_cfg.cmd.req_arg[CMD_EXPULSE] = 0;
	g_cfg.cmd.req_arg[CMD_BROADCAST] = 1;
	g_cfg.cmd.req_arg[CMD_INCANTATION] = 0;
	g_cfg.cmd.req_arg[CMD_FORK] = 0;
	g_cfg.cmd.req_arg[CMD_CONNECT_NBR] = 0;
}


