#include "zappy.h"
#include "../server.h"
#include "../logger.h"
#include "../utils.h"

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
	srv_event("pdi %d\n", player->id);
	srv_reply_client(player->id, "mort\n");
	srv_client_died(player->id);
	delete_player(player);
}


void	set_player_cell(t_player *player, t_cell *cell)
{
	log_debug("move visitor from cell(%d %d) to cell(%d %d)", 
			player->curr_cell->x, player->curr_cell->y,
			cell->x, cell->y);
	t_list *temp = ft_lstpop(&player->curr_cell->visitors, player);
	
	player->curr_cell->visitors_num--;
	ft_lstadd(&cell->visitors, temp);
	cell->visitors_num++;
	player->curr_cell = cell;
	log_debug("value of visitors of cell(%d %d): %p", cell->x, cell->y, player->curr_cell->visitors);
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
	if (!game->is_test) {
		srv_event("ppo %d %d %d %d\n",
			  player->id, new_x, new_y, player->orient);	
	}
}


/* Север 1, Восток 2, Юг 3, Запад 4
right: 1->2, 2->3, 3->4, 4->1
left: 1->4, 2->1, 3->2, 4->3
*/
static int turn(int o, int right)
{
	if (right) {
		++o;
		if (o == 5)
			o = 1;
	} else {
		--o;
		if (o == 0)
			o = 4;
	}
	return o;
}

/*
** turn right
*/
void	droite(t_player *player)
{
	player->orient = turn(player->orient, 1);
	t_buffer_write(game->buf, "ok");
	
	//тесты не поддерживают вывод инфы в монитор
	if (game->is_test)
		return ;

	srv_event("ppo %d %d %d %d\n",
			  player->id,
			  player->curr_cell->x, player->curr_cell->y,
			  player->orient);
}

/*
** turn left
*/
void	gauche(t_player *player)
{
	//player->orient = game->aux->orientation[(player->orient + 4 - 1) % 4 + 1];
	player->orient = turn(player->orient, 0);
	t_buffer_write(game->buf, "ok");

	//тесты не поддерживают вывод инфы в монитор
	if (game->is_test)
		return ;

	srv_event("ppo %d %d %d %d\n",
			  player->id,
			  player->curr_cell->x, player->curr_cell->y,
			  player->orient);
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
		for (int j = 0; j < cell->inventory[i]; j++) {
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
				int print_w = w;
				if (orient == ORIENT_S) {
					if (w < x) {
						print_w = x + (x - w);
					} else if (w > x) {
						print_w = x - (w - x);
					}
				}
				print_voir_cell(player, game->map->cells[h][get_w(print_w)]);
				/* printf("%d y %d x\n", h, get_w(print_w)); */
				w++;
				cells_counter++;
			}
		}
	} else if (orient == ORIENT_E || orient == ORIENT_W) {
		for (int i = 0; i < player->level + 1; i++) {

			int w = get_w(x + (orient == ORIENT_W ? -i : i));
			int h = max(y - i, 0);
			int max_h = min(y + 1 + i, map->h);
			
			while (h < max_h) {
				if (cells_counter) {
					t_buffer_write(game->buf, ", ");
				}
				int print_h = h;
				if (orient == ORIENT_W) {
					if (h < y) {
						print_h = y + (y - h);
					} else if (h > y) {
						print_h = y - (h - y);
					}
				}
				print_voir_cell(player, game->map->cells[get_h(print_h)][w]);
				h++;
				cells_counter++;
			}
			
		}
	} else {
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

		srv_event("pex %d\n"
				  ""			/* TODO pass 'ppo' for all expulsed players */
				  , player->id);
		
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

/*
static char *get_resource(char *data)
{
	return (NULL);
}
*/

void bct_srv_event(t_cell *cell)
{
	srv_event("bct %d %d %d %d %d %d %d %d %d\n",
			  cell->x, cell->y,
			  cell->inventory[0],
			  cell->inventory[1],
			  cell->inventory[2],
			  cell->inventory[3],
			  cell->inventory[4],
			  cell->inventory[5],
			  cell->inventory[6]
		);
}

static void	pin_bct_srv_event(t_player *player, int res, const char *cmd)
{
	srv_event("%s %d %d\n"
			  "pin %d %d %d %d %d %d %d %d %d %d\n",
			  cmd, player->id, res,
			  
			  player->id, player->curr_cell->x, player->curr_cell->y,
			  player->inventory[0],
			  player->inventory[1],
			  player->inventory[2],
			  player->inventory[3],
			  player->inventory[4],
			  player->inventory[5],
			  player->inventory[6]
		);
	bct_srv_event(player->curr_cell);
}

void	restore_resource(t_player *player, char *data)
{
	(void)player;
	char *resource_str = data + strlen(g_cfg.cmd.name[CMD_RESTORE_RESOURCE]) + 1;
	int resource_int = atoi(resource_str);
	t_cell *cell = get_random_cell(game->map);
	cell->inventory[resource_int]++;
	bct_srv_event(cell);
}

void	schedule_restore_resource(int resource, t_cell *cell)
{
	(void)cell;
	int is_food = resource == 0;

	if ((is_food && !FOOD_RESPAWN) || !is_food && !RESOURCE_RESPAWN) {
		return ;
	}

	int execute_time = is_food ? FOOD_RESPAWN_TIME : RESOURCE_RESPAWN_TIME;
	int buff_size = 20;
	char buf[buff_size];
	memset(buf, 0, buff_size);
	sprintf(buf, "%s %d", g_cfg.cmd.name[CMD_RESTORE_RESOURCE], resource);
	srv_push_command(0, buf, execute_time);
}

void	prend(t_player *player, char *data)
{
	char	*resource = data + strlen("prend ");
	int	resource_id = get_resource_id(resource);
  
	if (resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		t_buffer_write(game->buf, "ko");
	} else if (player->curr_cell->inventory[resource_id] > 0) {	
		player->curr_cell->inventory[resource_id]--;
		player->inventory[resource_id]++;
		t_buffer_write(game->buf, "ok");
		schedule_restore_resource(resource_id, player->curr_cell);
		//тесты не поддерживают вывод инфы в монитор
		if (game->is_test)
			return ;

		pin_bct_srv_event(player, resource_id, "pgt");
	} else {
		t_buffer_write(game->buf, "ko");
	}
}

void	pose(t_player *player, char *data)
{
	char	*resource = data + strlen("pose ");
	int	resource_id = get_resource_id(resource);

	if (resource_id == -1 || resource_id >= RESOURCES_NUMBER ||
		player->inventory[resource_id] < 1) {
		t_buffer_write(game->buf, "ko");
	} else {
		player->inventory[resource_id]--;
		player->curr_cell->inventory[resource_id]++;
		//t_buffer_json_message(game->buf, "OK");
		t_buffer_write(game->buf, "ok");

		//тесты не поддерживают вывод инфы в монитор
		if (game->is_test)
			return ;

		pin_bct_srv_event(player, resource_id, "pdr");
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
			quarter = 1;
		} else {
			quarter = 2;
		}
	} else {
		if (yr > ye) {
			quarter = 4;
		} else {
			quarter = 3;
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

	if (xe == xr && ye == yr) {
		return (SIDE_INNER);
	}
		

	int	x_min = min(xe, xr);
	int	y_min = min(ye, yr);
	int	x_max = max(xe, xr);
	int	y_max = max(ye, yr);

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
	//printf("%d %d %d %d \n", xe, xr, ye, yr);
	// is parallel
	if (xe == xr || ye == yr) {
		if (xe == xr) {
			if (ye < yr) {
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
	if (side == 0)
		return side;
	if (orient == ORIENT_E) {
		side += 2;
	} else if (orient == ORIENT_S) {
		side += 4;
	} else if (orient == ORIENT_W) {
		side += 6;
	}
	if (side > 8) {
		side -= 8;
	}
	return side;
}

void	broadcast(t_player *player, char *data)
{
	char	*text = data + strlen("broadcast ");

	int	size_pos = 8;
	t_buffer_write(game->buf, "message 0");
	t_buffer_add_char(game->buf, ',');
	t_buffer_write(game->buf, text);
	for (int i = 0; i < game->players_num; i++) {
		if (!game->players[i] || game->players[i]->id == player->id) {
			continue ;
		}
		int side = get_broadcast_side(player, game->players[i]);
		//printf("%d side\n", side);
		int side_normalized = normalize_side(game->players[i]->orient, side);
		//printf("%d normalized_side\n", side_normalized);
		game->buf->s[size_pos] = side_normalized + '0';
		reply_client(game->players[i]->id);
	}
	t_buffer_clean(game->buf);
	t_buffer_write(game->buf, "ok");

	//тесты не поддерживают вывод инфы в монитор
	if (game->is_test)
		return ;

	srv_event("pbc %d %s\n", player->id, text);
}

void	incantation(t_player *player)
{
	t_list *visitor = player->curr_cell->visitors;
	int	*incat_consts = game->aux->incantation[player->level - 1];
	int	incat_counter = incat_consts[RESOURCES_NUMBER_OF_PLAYERS] - 1;
	char event_str[512];
	char *event_str_ptr = event_str;

	event_str_ptr = &event_str[sprintf(event_str_ptr, "pic %d %d %d %d",
									   player->curr_cell->x,
									   player->curr_cell->y,
									   player->level + 1,
									   player->id)];

	/* check all players are same level */
	while (visitor && incat_counter) {
		t_player *p = (t_player *)visitor->content;
		if (p->level != player->level) {
			t_buffer_write(game->buf, "ko");
			return ;
		}
		if (p != player) {
			event_str_ptr = &event_str[sprintf(event_str_ptr, " %d", p->id)];
			--incat_counter;
		}
		visitor = visitor->next;
	}

	/* check number of players is valid */
	if (incat_counter > 0) {
		t_buffer_write(game->buf, "ko");
		return ;
	}

	/* check current cell contains all stones */
	for (int i = 1; i < RESOURCES_NUMBER; ++i) {
		if (player->curr_cell->inventory[i] < incat_consts[i]) {
			t_buffer_write(game->buf, "ko");
			return ;
		}
	}

	visitor = player->curr_cell->visitors;
	while (visitor) {
		t_player *p = (t_player *)visitor->content;
		if (p->is_incantating) {
			t_buffer_write(game->buf, "ko");
			return ;
		}
		p->is_incantating = 1;
		if (p != player)
			srv_reply_client(p->id, "elevation en cours\n");
		visitor = visitor->next;
	}

	srv_push_command(player->id,
					 g_cfg.cmd.name[CMD_INCANTATION_END],
					 g_cfg.cmd.duration[CMD_INCANTATION_END]);
		
	t_buffer_write(game->buf, "elevation en cours");

	if (game->is_test)
		return ;
	srv_event("%s\n", event_str);
}

void	incantation_end(t_player *player)
{
	int	*incat_consts = game->aux->incantation[player->level - 1];
	
	/* remove stones from cell */
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		player->curr_cell->inventory[i] -= incat_consts[i];
	}

	/* level up all visitors of player's cell */
	t_list *visitor = player->curr_cell->visitors;
	while (visitor) {
		t_player *p = (t_player *)visitor->content;
		if (p->is_incantating) {
			p->is_incantating = 0;
			++p->level;
			srv_event("plv %d %d\n", p->id, p->level);
			if (p != player) {
				srv_reply_client(p->id, "niveau actuel: %d\n", p->level);
			}
		}
		visitor = visitor->next;
	}
	t_buffer_write(game->buf, "niveau actuel: ");
	char new_level[] = "L";
	new_level[0] = player->level + '0';
	t_buffer_write(game->buf, new_level);
	if (game->is_test)
		return ;
	/* should return "ko" if players/stones combination 
	   not satisfy requirements, but we trust clients :) */
	srv_event("pie %d %d ok\n",
			  player->curr_cell->x, player->curr_cell->y);
	bct_srv_event(player->curr_cell);
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

static void	write_shift_pointer(char **dest, char *src)
{
	ft_memcpy(*dest, src, strlen(src));
	*dest += strlen(src);
}

void	hatch_egg(t_player *player)
{

}

static void send_egg_hatched(t_player *player)
{
	int buf_size = strlen(game->teams[player->team_id]->name) + 20;
	char buf[buf_size];
	char *curr_buf = buf;
	ft_memset(buf, 0, buf_size);
	char *cmd = "egg_hatched ";
	char *token = ft_itoa(game->hatchery->id_counter);

	write_shift_pointer(&curr_buf, cmd);
	write_shift_pointer(&curr_buf, game->teams[player->team_id]->name);
	write_shift_pointer(&curr_buf, " ");
	write_shift_pointer(&curr_buf, token);
	write_shift_pointer(&curr_buf, "\n");
	free(token);

	srv_push_command(0, buf, 600);
}

void	hatch_egg(t_player *player)
{
	
	send_egg_hatched(player);
}

void	do_fork(t_player *player)
{
	/*
	if (game->teams[player->team_id]->players_num == 6) {
		t_buffer_write(game->buf, "ko");
			return ;
	}
	*/
	return ;
	t_buffer_write(game->buf, "ok");
	srv_event("pfk %d\n", player->id);
	srv_push_command(0, "hatch_egg", 0);

	
	
	

	//
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
	g_cfg.cmd.duration[CMD_INCANTATION] = 0;
	g_cfg.cmd.duration[CMD_INCANTATION_END] = 300;
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
	g_cfg.cmd.name[CMD_INCANTATION_END] = strdup("incantation_end");
	g_cfg.cmd.name[CMD_FORK] = strdup("fork");
	g_cfg.cmd.name[CMD_CONNECT_NBR] = strdup("connect_nbr");
	g_cfg.cmd.name[CMD_RESTORE_RESOURCE] = strdup("restore_resource");

	g_cfg.cmd.f[CMD_AVANCE] = avance;
	g_cfg.cmd.f[CMD_DROITE] = droite;
	g_cfg.cmd.f[CMD_GAUCHE] = gauche;
	g_cfg.cmd.f[CMD_VOIR] = voir;
	g_cfg.cmd.f[CMD_INVENTAIRE] = inventory;
	g_cfg.cmd.f[CMD_EXPULSE] = expulse;
	g_cfg.cmd.f[CMD_INCANTATION] = incantation;
	g_cfg.cmd.f[CMD_INCANTATION_END] = incantation_end;
	g_cfg.cmd.f[CMD_FORK] = do_fork;
	g_cfg.cmd.f[CMD_CONNECT_NBR] = connect_nbr;

	g_cfg.cmd.f_arg[CMD_PREND] = prend;
	g_cfg.cmd.f_arg[CMD_POSE] = pose;
	g_cfg.cmd.f_arg[CMD_BROADCAST] = broadcast;
	g_cfg.cmd.f_arg[CMD_RESTORE_RESOURCE] = restore_resource;
	
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
	g_cfg.cmd.req_arg[CMD_RESTORE_RESOURCE] = 1;
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
	free(g_cfg.cmd.name[CMD_INCANTATION_END]);
	free(g_cfg.cmd.name[CMD_FORK]);
	free(g_cfg.cmd.name[CMD_CONNECT_NBR]);
	free(g_cfg.cmd.name[CMD_RESTORE_RESOURCE]);
	free(g_cfg.cmd.duration);
	free(g_cfg.cmd.name);
	free(g_cfg.cmd.f_arg);
	free(g_cfg.cmd.f);
	free(g_cfg.cmd.req_arg);
}
