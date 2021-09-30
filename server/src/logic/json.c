#include "zappy.h"

t_config config;

void	t_buffer_json_key(t_buffer *buf, char *key)
{
	t_buffer_add_char(buf, '\"');
	t_buffer_write(buf, key);
	t_buffer_write(buf, " : ");
	t_buffer_add_char(buf, '\"');
}

void	t_buffer_json_message(t_buffer *buf, char *message)
{
	t_buffer_write(buf, "{ ");
	t_buffer_json_key(buf, "message");
	t_buffer_write(buf, message);
	t_buffer_write(buf, " }");
}

void	write_cell_json(t_cell *cell, t_aux *aux)
{
	t_buffer_write(config.buf, "{ ");

	t_buffer_json_key(config.buf, "x");
	t_buffer_write_int(config.buf, cell->x);
	t_buffer_write(config.buf, ", ");

	t_buffer_json_key(config.buf, "y");
	t_buffer_write_int(config.buf, cell->y);
	t_buffer_write(config.buf, ", ");

	inventory(cell->inventory, aux);
	t_buffer_write(config.buf, ", ");

	t_buffer_json_key(config.buf, "players_num");
	t_buffer_write_int(config.buf, cell->visitors_num);
	t_buffer_write(config.buf, " }");
}

void	t_buffer_json_message_all(t_list *clients, t_buffer *buf, char *message, t_client *client)
{
	t_list		*tmp = clients;
	t_client	*tmp_client;

	while (tmp) {
		tmp_client = (t_client *)tmp->content;
		if (tmp_client != client) {
			t_buffer_json_message(buf, message);
		tmp = tmp->next;
		}
	}
}
