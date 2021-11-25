#include "zappy.h"
#include "../server.h"
#include "actions_client.h"
#include "../logger.h"

extern t_game *game;

int is_session_end(void)
{
	for (int i = 0; i < game->teams_num; i++)
	{
		if (game->teams[i]->max_level_count > 5)
		{
			return 1;
		}
	}
	return 0;
}

t_list *get_winners(void)
{
	t_list	*list = NULL;

	for (int i = 0; i < game->teams_num; i++)
	{
		if (game->teams[i]->max_level_count >=6)
		{
			ft_lstadd(&list, ft_lstnew_pointer(game->teams[i], sizeof(*game->teams[i])));
		}
	}
	return list;
}

void	starving_n_death(void)
{
	for (int i = 0; i < game->players_num; i++) {
		if (!game->players[i]) {
			continue ;
		}
		if (game->curr_tick - game->players[i]->last_meal_tick > 125) {
			game->players[i]->inventory[NOURRITURE]--;
			game->players[i]->last_meal_tick = game->curr_tick;
			srv_event("pin %d %d %d %d %d %d %d %d %d %d\n",
						game->players[i]->id,
						game->players[i]->curr_cell->x,
						game->players[i]->curr_cell->y,
						game->players[i]->inventory[0],
						game->players[i]->inventory[1],
						game->players[i]->inventory[2],
						game->players[i]->inventory[3],
						game->players[i]->inventory[4],
						game->players[i]->inventory[5],
						game->players[i]->inventory[6]);
		}
		//log_debug("player[%d].food = %d", i, game->players[i]->inventory[0]);
		if (game->players[i]->inventory[0] == 0) {
			mort(game->players[i]);
		}
	}
}
