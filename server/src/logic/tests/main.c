#include "../zappy.h"
#include "../../server.h"

static char *default_teams[] = {"team1", "team2", "team3"};
t_main_config g_cfg = {
	.port = 9876,
	.width = 10,
	.height = 10,
	.teams = default_teams,
	.teams_count = 3,
	.max_clients_at_team = 5,
	.t = 1,
	.quiet = 0,
	.cmd = {NULL, NULL, NULL, NULL, NULL}
};	

extern t_game	*game;

void	test_broadcast() {
	t_player	*emitter = create_player(0,0);
	t_player	*receiver1 = create_player(1,0);
	t_player	*receiver2 = create_player(2,0);

	char *msg = strdup("test");
}

int main() {
	lgc_init();
	
	/*
	int h = 10;
	int w = 10;
	
	//game is global 
	game = create_game();
	game->players_num = 2;
	game->teams_num = 2;
	
	
	
	exit(0);
	//t_cell *temp = get_random_cell(map);
	t_cell *temp = game->map->cells[1][1];
	t_player *player = create_player(1); 
	
	add_visitor(temp, player);
	print_map(game, print_player);
	printf("%d before\n", player->orient);
	gauche(player, game->aux);
	printf("%d after\n", player->orient);
	print_map(game, print_player);

	test_broadcast();
	*/
	return (0);
}


