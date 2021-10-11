
#include "../zappy.h"
#include "../../server.h"
#include "../../logic.h"
#include "../../utils.h"
#include "../color.h"

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

void	test_avance()
{
	int player_id = 3;
	char *cmd = "avance";

	t_player *player = add_player(player_id, 5);
	print_player(player);
	set_player_cell(player, game->map->cells[0][0]);
	
	player->orient = ORIENT_N;
	print_player(player);
	
	// must be x=0 y=9
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_avance");
	}
	printf("%d\n", cmd_id);
	lgc_execute_command(player_id, cmd, cmd_id);
	
	xassert(player->curr_cell->x == 0 && player->curr_cell->y == g_cfg.height - 1, "avance ORIENT N FAILED");
	print_player(player);
	exit(0);
	// must be 9 9
	gauche(player);
	xassert(player->orient == ORIENT_W, "gauche FAILED");
	avance(player);
	xassert(player->curr_cell->x == 9 && player->curr_cell->y == g_cfg.height - 1, "avance ORIENT W FAILED");
	print_player(player);

	//must be x=9 y=0
	droite(player);
	droite(player);
	droite(player);
	xassert(player->orient == ORIENT_S, "triple droite FAILED");
	
	avance(player);
	xassert(player->curr_cell->x == 9 && player->curr_cell->y == 0, "avance ORIENT S FAILED");
	print_player(player);

	//must be x=0 y=0
	gauche(player);
	avance(player);
	xassert(player->curr_cell->x == 0 && player->curr_cell->y == 0, "avance ORIENT E FAILED");
	print_player(player);
}

void	test_prend()
{
	t_player *player = add_player(3, 5);
	print_player(player);
	set_player_cell(player, game->map->cells[0][0]);
	player->orient = ORIENT_N;
	print_player(player);

	
}


int main() {
	
	init_cmd();	
	lgc_init();
	
	game->is_test = 1;
	test_avance();

	
	printf("%sTESTS PASSED SUCCESSFULLY IF NO ERRORS OCCURED%s\n",
		ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
	exit(0);
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


