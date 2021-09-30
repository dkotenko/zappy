#include "zappy.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

t_config config;
void	get_forward_coords(t_client *client, int *new_x, int *new_y);

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

//NOT TESTED
void	starving_n_death(t_game *game)
{
	t_list		*tmp;
	t_client	*tmp_client;

	tmp = game->clients;
	while (tmp) {
		tmp_client = (t_client *)game->clients->content;
		tmp_client->hp--;
		if (!tmp_client->hp) {
			t_list *tmp_list = ft_listpop(game->clients, tmp);
			tmp_client = (t_client *)tmp_list->content;
			for (int i = 1; i < RESOURCES_NUMBER; i++) {
				tmp_client->curr_cell->inventory[i] += tmp_client->inventory[i];
			}
			free(tmp_client->inventory);
			free(tmp_client);
			free(tmp_list);
		}
		tmp = tmp->next;
	}
}



void	avanche(t_map *map, t_client *client)
{
	int	new_x = 0;
	int new_y = 0;

	get_forward_coords(client, &new_x, &new_y);
	t_list *temp = ft_listpop(client->curr_cell->visitors, client);
	map->cells[new_y][new_x]->visitors->next = temp;
	t_buffer_json_message(config.buf, "OK");
}


//TESTED
//returns OK
void	droite(t_client *client, t_aux *aux)
{
	
	client->orient = aux->orientation[(client->orient + 1) % 4];

	t_buffer_json_message(config.buf, "OK");
}


//TESTED
//returns OK
void	gauche(t_client *client, t_aux *aux)
{
	client->orient = aux->orientation[(client->orient + 4 - 1) % 4];
	t_buffer_json_message(config.buf, "OK");
}


//returns items
void	inventory(int *inv, t_aux *aux)
{
	t_buffer_write(config.buf, "{ ");
	t_buffer_json_key(config.buf, "inventory");
	t_buffer_write(config.buf, "{ ");
	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		t_buffer_json_key(config.buf, aux->resources[i]);
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
void	voir(t_client *client, t_map *map, t_aux *aux)
{	
	int printed[map->h][map->w];
	memset(printed, 0, sizeof(printed));
	int x = client->curr_cell->x;
	int y = client->curr_cell->y;
	
	int orient = client->orient;
	if (orient == ORIENT_N || orient == ORIENT_S) {
		for (int i = 0; i < client->level + 1; i++) {
			int h = get_h(y + (orient == ORIENT_S ? i : -i));
			int w = max(x - i, 0);
			int max_w = min(x + 1 + i, map->w);
			while (w < max_w) {
				printed[h][get_w(w)] = 1;
				w++;
			}
		}
	} else if (orient == ORIENT_W || orient == ORIENT_E) {
		for (int i = 0; i < client->level + 1; i++) {
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
				write_cell_json(map->cells[i][j], aux);
			}
		}
	}
	t_buffer_write(config.buf, "]}");
}

void	get_forward_coords(t_client *client, int *new_x, int *new_y)
{
	int x = client->curr_cell->x;
	int y = client->curr_cell->y;

	if (client->orient == ORIENT_E)
	{
		*new_x = get_x(client->curr_cell->x + 1);
		*new_y = y;
	}
	else if (client->orient == ORIENT_N)
	{
		*new_x = x;
		*new_y = get_y(client->curr_cell->y + 1);
	}
	else if (client->orient == ORIENT_W)
	{
		*new_x = get_x(client->curr_cell->x - 1);
		*new_y = y;
	}
	else if (client->orient == ORIENT_S)
	{
		*new_x = x;
		*new_y = get_y(client->curr_cell->y - 1);
	}
	else {
		handle_error("expulse error: invalid orient");
	}
}

//return ok//ko
void	expulse(t_client *client, t_map *map)
{
	int	new_x = 0;
	int new_y = 0;
	
	get_forward_coords(client, &new_x, &new_y);
	t_list *temp = ft_listpop(client->curr_cell->visitors, client);
	if (client->curr_cell->visitors) {
		map->cells[new_y][new_x]->visitors->next = client->curr_cell->visitors;
		t_buffer_json_message(config.buf, "OK");
		t_buffer_json_message_all(client->curr_cell->visitors, config.buf, "deplacement", client);
	} else {
		t_buffer_json_message(config.buf, "KO");
	}
	client->curr_cell->visitors = temp;
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
void	prend(char *resource, t_client *client, t_aux *aux)
{
	int	resource_id;

	resource_id = get_resource_id(aux, resource);
	if (resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		t_buffer_json_message(config.buf, "KO");
	} else if (client->curr_cell->inventory[resource_id] > 0)
	{
		client->curr_cell->inventory[resource_id]--;
		client->inventory[resource_id]++;
		t_buffer_json_message(config.buf, "OK");
	} else {
		t_buffer_json_message(config.buf, "KO");
	}
}


//NOT TESTED
void	pose(char *resource, t_client *client, t_aux *aux)
{
	int	resource_id;

	resource_id = get_resource_id(aux, resource);
	if (resource_id == -1 || resource_id >= RESOURCES_NUMBER) {
		t_buffer_json_message(config.buf, "KO");
	} else if (client->inventory[resource_id] > 0) {
		client->inventory[resource_id]--;
		client->curr_cell->inventory[resource_id]++;
		t_buffer_json_message(config.buf, "OK");
	} else {
		t_buffer_json_message(config.buf, "KO");
	}
}

/*
void	broadcast(t_game *game, t_client *client, char *text)
{
	
}
*/

//NOT TESTED
void	incantation(t_game *game, t_client *client)
{
	t_list *temp = client->curr_cell->visitors;
	int	*incat_consts = game->aux->incantation[client->level - 1];

	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		if (client->inventory[i] < incat_consts[i]) {
			t_buffer_json_message(config.buf, "KO");
			return ;	
		}
	}

	int	incat_counter = incat_consts[RESOURCES_NUMBER_OF_PLAYERS] - 1;
	t_client *participants[incat_counter];
	while (temp && incat_counter) {
		t_client *temp_client = (t_client *)temp->content;
		if (temp_client != client && temp_client->level == client->level) {
			participants[--incat_counter] = temp_client;
		}
		temp = temp->next;
	}
	if (incat_counter) {
		t_buffer_json_message(config.buf, "KO");
		return ;
	}

	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		client->inventory[i] -= incat_consts[i];
	}

	for (int i = 0; i < incat_consts[RESOURCES_NUMBER_OF_PLAYERS] - 1; i++) {
		participants[i]->level++;
	}
	client->level++;
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

/*
void	do_fork(t_client *client)
{
	add_client(create_client);
	client->is_egg = 1;
	//t_buf write OK, write token
}
*/

void	connect_nbr()
{
}



