
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
int	g_tests_result;


/*
void	test_broadcast() {
	t_player	*emitter = create_player(0,0);
	t_player	*receiver1 = create_player(1,0);
	t_player	*receiver2 = create_player(2,0);

	
	char *msg = strdup("test");

}
*/

void	test_avance()
{
	int player_id = 3;
	char *cmd = "avance";
	int local_test_result = 0;

	t_player *player = add_player(player_id, 5);
	set_player_cell(player, game->map->cells[0][0]);
	
	player->orient = ORIENT_N;
	print_player(player);
	
	// must be x=0 y=9
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_avance");
	}
	
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	
	local_test_result += xassert(player->curr_cell->x == 0 && player->curr_cell->y == g_cfg.height - 1, "avance ORIENT N FAILED");
	
	// must be 9 9
	gauche(player);
	local_test_result += xassert(player->orient == ORIENT_W, "gauche FAILED");
	avance(player);
	local_test_result += xassert(player->curr_cell->x == 9 && player->curr_cell->y == g_cfg.height - 1, "avance ORIENT W FAILED");
	
	//must be x=9 y=0
	droite(player);
	droite(player);
	droite(player);
	local_test_result += xassert(player->orient == ORIENT_S, "triple droite FAILED");
	
	avance(player);
	local_test_result += xassert(player->curr_cell->x == 9 && player->curr_cell->y == 0, "avance ORIENT S FAILED");

	//must be x=0 y=0
	gauche(player);
	avance(player);
	local_test_result += xassert(player->curr_cell->x == 0 && player->curr_cell->y == 0, "avance ORIENT E FAILED");
	g_tests_result += local_test_result;
}

void	test_prend()
{
	t_player *player = add_player(3, 5);
	int player_id = 3;
	char *cmd = "avance";
	int local_test_result = 0;

	
	print_player(player);

	
}

void	test_inventoire()
{
	t_player *player = add_player(3, 5);
	int player_id = 3;
	char *cmd = "inventaire";
	int local_test_result = 0;

	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_inventoire");
	}
	
	print_player(player);
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	
}

int main() {
	
	g_tests_result = 0;
	init_cmd();
	
	lgc_init();
	game->is_test = 1;
	
	//test_avance();
	test_inventoire();

	if (g_tests_result == 0) {
		printf("%sTESTS PASSED SUCCESSFULLY%s\n",
		ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
	} else {
		printf("%sTESTS FAILED%s\n",
		ANSI_COLOR_RED, ANSI_COLOR_RESET);
	}
	
	exit(0);
	return (0);
}


