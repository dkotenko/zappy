#include "zappy.h"

t_config config;

void	t_buffer_json_key(t_buffer *buf, char *key)
{
	t_buffer_add_char(buf, '\"');
	t_buffer_write(buf, key);
	t_buffer_write(buf, "\" : ");
}

void	t_buffer_json_message(t_buffer *buf, char *message)
{
	t_buffer_write(buf, "{ ");
	printf("\n%s\n", buf->s);
	t_buffer_json_key(buf, "message");
	printf("\n%s\n", buf->s);
	t_buffer_write(buf, message);
	printf("\n%s\n", buf->s);
	t_buffer_write(buf, " }");
	printf("\n%s\n", buf->s);
}

void	write_cell_json(t_cell *cell)
{
	t_buffer_write(config.buf, "{ ");

	t_buffer_json_key(config.buf, "x");
	t_buffer_write_int(config.buf, cell->x);
	t_buffer_write(config.buf, ", ");

	t_buffer_json_key(config.buf, "y");
	t_buffer_write_int(config.buf, cell->y);
	t_buffer_write(config.buf, ", ");

	inventory(cell->inventory);
	t_buffer_write(config.buf, ", ");

	t_buffer_json_key(config.buf, "players_num");
	t_buffer_write_int(config.buf, cell->visitors_num);
	t_buffer_write(config.buf, " }");
}

void	t_buffer_json_message_all(t_list *players, t_buffer *buf, char *message, t_player *player)
{
	t_list		*tmp = players;
	t_player	*tmp_player;

	while (tmp) {
		tmp_player = (t_player *)tmp->content;
		if (tmp_player != player) {
			t_buffer_json_message(buf, message);
		tmp = tmp->next;
		}
	}
}

void	inventory_json(int *inv)
{
	t_buffer_write(game->buf, "{ ");
	t_buffer_json_key(game->buf, "inventory");
	t_buffer_write(game->buf, "{ ");
	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		t_buffer_json_key(game->buf, game->aux->resources[i]);
		t_buffer_write_int(game->buf, inv[i]);
		if (i != RESOURCES_NUMBER - 1) {
			t_buffer_write(game->buf, ", ");
		}
	}
	t_buffer_write(game->buf, " }}");
}

void	print_voir_json()
{
	t_map *map = game->map;
	t_buffer_write(game->buf, "{ ");
	t_buffer_json_key(game->buf, "cells");
	t_buffer_add_char(game->buf, '[');
	for (int i = 0; i < map->h; i++) {
		for (int j = 0; j < map->w; j++) {
			/*
			if (printed[i][j]) {
				write_cell_json(map->cells[i][j]);
			}
			*/
		}
	}
	t_buffer_write(game->buf, "]}");
}