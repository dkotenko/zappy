#include "zappy.h"
#include "../server.h"
#include "../logger.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))


extern t_game *game;

void	get_forward_coords(t_player *player, int *new_x, int *new_y);

int		get_x(int coord)
{
	if (coord < 0)
		coord += g_cfg.width;
	else if (coord >= g_cfg.width)
		coord -= g_cfg.width;
	return coord;
}

int		get_y(int coord)
{
	if (coord < 0)
		coord += g_cfg.height;
	else if (coord >= g_cfg.height)
		coord -= g_cfg.height;
	return coord;
}

void	mort(t_player *player)
{
	delete_player(player);
	t_buffer_write(game->buf, "mort");
}



void	set_player_cell(t_player *player, t_cell *cell)
{
	t_list *temp = ft_lstpop(&player->curr_cell->visitors, player);
	player->curr_cell->visitors_num--;
	ft_lstadd(&cell->visitors, temp);
	cell->visitors_num++;
	player->curr_cell = cell;
}


/*
** move forward
*/
void	avance(t_player *player)
{
	int	new_x = 0;
	int new_y = 0;

	get_forward_coords(player, &new_x, &new_y);
	t_cell *cell = game->map->cells[new_y][new_x];
	set_player_cell(player, cell);
	t_buffer_write(game->buf, "ok");
}

/*
** turn right
*/
void	droite(t_player *player)
{
	player->orient = game->aux->orientation[(player->orient + 1) % 4];
	t_buffer_write(game->buf, "ok");
}

/*
** turn left
*/
void	gauche(t_player *player)
{
	player->orient = game->aux->orientation[(player->orient + 4 - 1) % 4];
	t_buffer_write(game->buf, "ok");
}

/*
** returns items
*/
void	inventory(t_player *player)
{
	
	t_buffer_write(game->buf, "{");
	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		t_buffer_write(game->buf, game->aux->resources[i]);
		t_buffer_add_char(game->buf, ' ');
		t_buffer_write_int(game->buf, player->inventory[i]);
		if (i != RESOURCES_NUMBER - 1) {
			t_buffer_write(game->buf, ", ");
		}
	}
	t_buffer_write(game->buf, "}");
}

int		get_w(int coord)
{
	if (coord < 0)
		coord += g_cfg.width;
	else if (coord >= g_cfg.width)
		coord -= g_cfg.width;
	return coord;
}

int		get_h(int coord)
{
	if (coord < 0)
		coord += g_cfg.height;
	else if (coord >= g_cfg.height)
		coord -= g_cfg.height;
	return coord;
}

static void	print_voir_cell(t_player *player, t_cell *cell)
{
	int	printed = 0;
	t_list *temp = cell->visitors;

	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		for (int j =0; j < cell->inventory[i]; j++) {
			if (printed) {
				t_buffer_add_char(game->buf, ' ');
			}
			printed = 1;
			t_buffer_write(game->buf, game->aux->resources[i]);
		}
	}
	
	while (temp) {
		if ((t_player *)temp->content != player) {
			if (printed) {
				t_buffer_add_char(game->buf, ' ');
			}
			printed = 1;
			t_buffer_write(game->buf, "player");
		}
		temp = temp->next;
	}
}

/*
** returns cells
*/ 
void	voir(t_player *player)
{	
	t_map *map = game->map;

	//int printed[map->h][map->w];
	//memset(printed, 0, sizeof(printed));
	int x = player->curr_cell->x;
	int y = player->curr_cell->y;
	int orient = player->orient;
	int cells_counter = 0;

	t_buffer_write(game->buf, "{");
	if (orient == ORIENT_N || orient == ORIENT_S) {
		for (int i = 0; i < player->level + 1; i++) {

			int h = get_h(y + (orient == ORIENT_S ? i : -i));
			int w = max(x - i, 0);
			int max_w = min(x + 1 + i, map->w);
			
			while (w < max_w) {
				if (cells_counter) {
					t_buffer_write(game->buf, ", ");
				}
				print_voir_cell(player, game->map->cells[h][get_w(w)]);
				//printed[h][get_w(w)] = 1;
				w++;
				cells_counter++;
			}
			
		}
	} else if (orient == ORIENT_W || orient == ORIENT_E) {
		for (int i = 0; i < player->level + 1; i++) {

			int w = get_w(x + (orient == ORIENT_W ? -i : i));
			int h = max(y - i, 0);
			int max_h = min(y + 1 + i, map->h);
			
			while (h < max_h) {
				if (cells_counter) {
					t_buffer_write(game->buf, ", ");
				}
				//printed[get_h(h)][w] = 1;
				//printf("x:%d y:%d\n", w, h);
				print_voir_cell(player, game->map->cells[get_h(h)][w]);
				h++;
				cells_counter++;
			}
			
		}
	}
	else {
		log_warning("Invalid orientation");
	}
	t_buffer_write(game->buf, "}");
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
		*new_y = get_y(player->curr_cell->y - 1);
	}
	else if (player->orient == ORIENT_W)
	{
		*new_x = get_x(player->curr_cell->x - 1)
		;
		*new_y = y;
	}
	else if (player->orient == ORIENT_S)
	{
		*new_x = x;
		*new_y = get_y(player->curr_cell->y + 1);
	}
	else {
		log_warning("expulse error: invalid orient");
	}
}

/*
** returns ok/ko
*/
void	expulse(t_player *player)
{
	int	new_x = 0;
	int new_y = 0;
	t_list *temp = NULL;
	
	get_forward_coords(player, &new_x, &new_y);
	if (player->curr_cell->visitors_num > 1) { /* TODO check visitors_num is valid */
		temp = ft_lstpop(&player->curr_cell->visitors, player);
		game->map->cells[new_y][new_x]->visitors->next = player->curr_cell->visitors;
		t_buffer_write(game->buf, "ok");
		reply_and_clean_buff(player->id);
		t_buffer_write(game->buf, "deplacement");
		reply_except_list(player->curr_cell->visitors, player->id);
	} else {
		t_buffer_write(game->buf, "ko");
		reply_and_clean_buff(player->id);
	}
	player->curr_cell->visitors = temp;
}


static int	get_resource_id(char *resource)
{
	int	i;

	i = -1;
	while (++i < 7)
	{
		if (!strcmp(resource, game->aux->resources[i]))
			return i;
	}
	return (-1);
}

//NOT TESTED
void	prend(t_player *player, char *data)
{
	char	*resource = data + strlen("prend ");
	int	resource_id = get_resource_id(resource);
	
	if ((!resource_id && resource[0] != '0') || resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		//t_buffer_json_message(game->buf, "KO");
		t_buffer_write(game->buf, "ko");
	} else if (player->curr_cell->inventory[resource_id] > 0)
	{
		player->curr_cell->inventory[resource_id]--;
		player->inventory[resource_id]++;
		//t_buffer_json_message(game->buf, "OK");
		t_buffer_write(game->buf, "ok");
	} else {
		//t_buffer_json_message(game->buf, "KO");
		t_buffer_write(game->buf, "ko");
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
	int	resource_id = get_resource_id(resource);

	if ((!resource_id && resource[0] != '0') \
	|| resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		//t_buffer_json_message(game->buf, "KO");
		t_buffer_write(game->buf, "ko");
	} else if (player->inventory[resource_id] > 0) {
		player->inventory[resource_id]--;
		player->curr_cell->inventory[resource_id]++;
		//t_buffer_json_message(game->buf, "OK");
		t_buffer_write(game->buf, "ok");
	} else {
		//t_buffer_json_message(game->buf, "KO");
		t_buffer_write(game->buf, "ko");
	}
}

enum e_sides {
	SIDE_INNER,
	SIDE_UP,
	SIDE_UPLEFT,
	SIDE_LEFT,
	SIDE_DOWNLEFT,
	SIDE_DOWN,
	SIDE_DOWNRIGHT,
	SIDE_RIGHT,
	SIDE_UPRIGHT
};

	/*
	** quarters for receiver
	**	1_|_4
	** 	2 |	3
	*/
int get_quarter(int xr, int xe, int yr, int ye) {
	int quarter = 0;
	
	if (xr > xe) {
		if (yr > ye) {
			quarter = 2;
		} else {
			quarter = 1;
		}
	} else {
		if (yr > ye) {
			quarter = 3;
		} else {
			quarter = 4;
		}
	}
	return (quarter);
}

int		get_broadcast_side(t_player *emitter, t_player *receiver)
{
	int	xe = emitter->curr_cell->x;
	int ye = emitter->curr_cell->y;
	int xr = receiver->curr_cell->x;
	int yr = receiver->curr_cell->y;

	if (xe == ye && xr == yr)
		return (SIDE_INNER);

	int	x_min = min(xe, xr);
	int	y_min = min(ye, yr);
	int	y_max = max(xe, xr);
	int	x_max = max(ye, yr);

	if (abs(xe - xr) > x_min + g_cfg.width - x_max) {
		if (xe == x_min) {
			xe += g_cfg.width;
		} else {
			xr += g_cfg.width;
		}
	}
	if (abs(ye - yr) > y_min + g_cfg.height - y_max) {
		if (ye == y_min) {
			ye += g_cfg.height;
		} else {
			yr += g_cfg.height;
		}
	}
	
	// is parallel
	if (xe == xr || ye == yr) {
		if (xe == xr) {
			if (ye > yr) {
				return SIDE_UP;
			} else {
				return SIDE_DOWN;
			}
		} else {
			if (xe > xr) {
				return SIDE_RIGHT;
			} else {
				return SIDE_LEFT;
			}
		}
	}

	int quarter = get_quarter(xr, xe, yr, ye);
	int abs_x = abs(xe - xr);
	int abs_y = abs(ye - yr);
	//is diagonal
	if (abs_x == abs_y) {
		return quarter * 2;
	}

	if (abs_x > abs_y) {
		if (quarter == 1 || quarter == 2) {
			return SIDE_LEFT;
		} else {
			return SIDE_RIGHT;
		}
	} else {
		if (quarter == 1 || quarter == 4) {
			return SIDE_UP;
		} else {
			return SIDE_DOWN;
		}
	}
}

int		normalize_side(int orient, int side)
{
	if (orient == ORIENT_E) {
		side += 6;
	} else if (orient == ORIENT_S) {
		side += 4;
	} else if (orient == ORIENT_W) {
		side += 2;
	}
	if (side > 8) {
		side -= 8;
	}
	return side;
}

void	broadcast(t_player *player, char *data)
{
	char	*text = data + strlen("broadcast ");
	t_player	*receiver = NULL;

	int	size_pos = 8;
	t_buffer_write(game->buf, "message 0");
	t_buffer_add_char(game->buf, ',');
	t_buffer_write(game->buf, text);
	for (int i = 0; i < game->players_num; i++) {
		if (!game->players[i] || game->players[i]->id == player->id) {
			continue ;
		}
		int side = get_broadcast_side(player, receiver);
		int side_normalized = normalize_side(game->players[i]->orient, side);
		game->buf->s[size_pos] = side_normalized + '0';
		srv_reply_client(game->players[i]->id, "%s\n", game->buf->s);
	}
	t_buffer_clean(game->buf);
}

void	incantation(t_player *player)
{
	t_list *temp = player->curr_cell->visitors;
	int	*incat_consts = game->aux->incantation[player->level - 1];

	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		if (player->inventory[i] < incat_consts[i]) {
			t_buffer_write(game->buf, "ko");
			//t_buffer_json_message(game->buf, "ko");
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
		//t_buffer_json_message(game->buf, "ko");
		t_buffer_write(game->buf, "ko");
		return ;
	}

	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		player->inventory[i] -= incat_consts[i];
	}

	for (int i = 0; i < incat_consts[RESOURCES_NUMBER_OF_PLAYERS] - 1; i++) {
		participants[i]->level++;
		if (participants[i]->level == PLAYER_MAX_LEVEL) {
			game->teams[participants[i]->team_id]->max_level_count++;
		}
	}
	player->level++;
	if (player->level == PLAYER_MAX_LEVEL) {
		game->teams[player->team_id]->max_level_count++;
	}
}

/*
** 8-symbol token, symbols in range [A : Z]
*/
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
	//game->teams[player->team_id]->
	//
	
	//add_player(pla);
	//player->is_egg = 1;
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
	xassert(g_cfg.cmd.f_arg != NULL, "malloc");

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

	g_cfg.cmd.f[CMD_AVANCE] = avance;
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

void clear_cmd()
{
	free(g_cfg.cmd.name[CMD_AVANCE]);
	free(g_cfg.cmd.name[CMD_DROITE]);
	free(g_cfg.cmd.name[CMD_GAUCHE]);
	free(g_cfg.cmd.name[CMD_VOIR]);
	free(g_cfg.cmd.name[CMD_INVENTAIRE]);
	free(g_cfg.cmd.name[CMD_PREND]);
	free(g_cfg.cmd.name[CMD_POSE]);
	free(g_cfg.cmd.name[CMD_EXPULSE]);
	free(g_cfg.cmd.name[CMD_BROADCAST]);
	free(g_cfg.cmd.name[CMD_INCANTATION]);
	free(g_cfg.cmd.name[CMD_FORK]);
	free(g_cfg.cmd.name[CMD_CONNECT_NBR]);
	free(g_cfg.cmd.duration);
	free(g_cfg.cmd.name);
	free(g_cfg.cmd.f_arg);
	free(g_cfg.cmd.f);
	free(g_cfg.cmd.req_arg);
}
