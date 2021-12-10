#include "zappy.h"
#include "../server.h"
#include "actions_client.h"
#include "../logger.h"
#include "../utils.h"

extern t_game *game;

void	end_game(void)
{
	t_list *winners = get_winners();

	xassert(winners != NULL, "no winners, but end_game started");
	t_team *team = (t_team *)winners->content;
	srv_event("seg %d\n", team->id);
	for (int i = 0; i < game->players_size; i++) {
		if (game->players[i]) {
			mort(game->players[i]);
		}
	}
	char s[1024] = {0};
	while (strcmp(s, "yes") && strcmp(s, "y")) {		
		scanf("%s\n", s);
	}
	return ;
}

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
			ft_lstadd(&list, ft_lstnew_pointer(game->teams[i], sizeof(t_team)));
		}
	}
	return list;
}

void	starve_players()
{
	int count = 0;
	for (int id = 0; id < game->players_size && count < game->players_num; ++id) {
		t_player *player = game->players[id];
		if (!player)
			continue ;
		if (game->curr_tick - player->last_meal_tick >= TICKS_FOR_STARVE) {	
			player->inventory[NOURRITURE]--;
			player->last_meal_tick = game->curr_tick;
			srv_event("pin %d %d %d %d %d %d %d %d %d %d\n",
					player->id,
					player->curr_cell->x,
					player->curr_cell->y,
					player->inventory[0],
					player->inventory[1],
					player->inventory[2],
					player->inventory[3],
					player->inventory[4],
					player->inventory[5],
					player->inventory[6]);
		}

		if (player->inventory[0] == 0) {
			mort(player);
		}
		++count;
	}
}

/*
void	starve_players_()
{
	for (int i = 0; i < game->teams_num; i++) {
		t_list *tmp = game->teams[i]->players;

		while (tmp) {
			t_player *player_copy = (t_player *)tmp->content;
			t_player *player = game->players[player_copy->id];
			if (game->curr_tick - player->last_meal_tick >= TICKS_FOR_STARVE) {
				player->inventory[NOURRITURE]--;
				player->last_meal_tick = game->curr_tick;
				srv_event("pin %d %d %d %d %d %d %d %d %d %d\n",
						player->id,
						player->curr_cell->x,
						player->curr_cell->y,
						player->inventory[0],
						player->inventory[1],
						player->inventory[2],
						player->inventory[3],
						player->inventory[4],
						player->inventory[5],
						player->inventory[6]);
			}

			if (player->inventory[0] == 0) {
				mort(player);
			}
			tmp = tmp->next;
		}
	}
}
*/

void	starve_eggs()
{
	t_list *tmp = game->hatchery->eggs;
	while (tmp) {
		t_egg *egg = (t_egg *)tmp->content;
		if (game->curr_tick - egg->last_meal_tick >= TICKS_FOR_STARVE) {
			egg->nourriture--;
			egg->last_meal_tick = game->curr_tick;			
		}

		if (egg->nourriture == 0) {
			mort_egg(egg);
		}
	}
}

void	starving_n_death(void)
{
	starve_players();
	/* starve_eggs(); */
}
