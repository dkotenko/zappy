
#include "../zappy.h"
#include "../../server.h"
#include "../../logic.h"
#include "../../utils.h"
#include "../color.h"

void	init_game(void);

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

extern	t_game	*game;
int		g_tests_result;
t_list	*g_list_game_to_free;
t_dlist	*g_messages;

void	test_avance()
{
	init_game();
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
	lgc_execute_command(player_id, "gauche", lgc_get_command_id("gauche"));
	local_test_result += xassert(player->orient == ORIENT_W, "gauche FAILED");
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	local_test_result += xassert(player->curr_cell->x == 9 && player->curr_cell->y == g_cfg.height - 1, "avance ORIENT W FAILED");
	
	//must be x=9 y=0
	lgc_execute_command(player_id, "droite", lgc_get_command_id("droite"));
	lgc_execute_command(player_id, "droite", lgc_get_command_id("droite"));
	lgc_execute_command(player_id, "droite", lgc_get_command_id("droite"));
	local_test_result += xassert(player->orient == ORIENT_S, "triple droite FAILED");
	
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	local_test_result += xassert(player->curr_cell->x == 9 && player->curr_cell->y == 0, "avance ORIENT S FAILED");

	//must be x=0 y=0
	lgc_execute_command(player_id, "gauche", lgc_get_command_id("gauche"));
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	local_test_result += xassert(player->curr_cell->x == 0 && player->curr_cell->y == 0, "avance ORIENT E FAILED");
	g_tests_result += local_test_result;
}

void	test_prend()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	char *cmd = "prend nourriture";
	int local_test_result = 0;
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	
	print_player(player);
	ft_memset(player->curr_cell->inventory, 0, sizeof(int) * RESOURCES_NUMBER);
	player->curr_cell->inventory[NOURRITURE] = 1;
	int player_nourriture = player->inventory[NOURRITURE];
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	local_test_result += xassert(player->inventory[NOURRITURE] == player_nourriture + 1, "no item in player inv after prend");
	local_test_result += xassert(player->curr_cell->inventory[NOURRITURE] == 0, "prend: item remains at cell");

	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
}

void	test_pose()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	char *cmd = "pose nourriture";
	int local_test_result = 0;
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	
	print_player(player);
	ft_memset(player->inventory, 0, sizeof(int) * RESOURCES_NUMBER);
	player->inventory[NOURRITURE] = 1;
	int cell_nourriture = player->curr_cell->inventory[NOURRITURE];
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	print_cell(player->curr_cell);
	local_test_result += xassert(player->curr_cell->inventory[NOURRITURE] == cell_nourriture + 1, "no item left from player inv after pose");
	local_test_result += xassert(player->inventory[NOURRITURE] == 0, "pose: item remains at player inv");

	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
}


void	test_inventoire()
{
	t_player *player = add_player(3, 5);
	set_player_cell(player, game->map->cells[0][0]);
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

void	test_voir()
{
	int player_id = 3;
	t_player *player = add_player(player_id, 5);
	set_player_cell(player, game->map->cells[0][0]);
	player->orient = ORIENT_E;
	t_player *player2 = add_player(0, 5);
	t_player *player3 = add_player(1, 5);
	set_player_cell(player2, game->map->cells[0][1]);
	set_player_cell(player3, game->map->cells[0][1]);
	//print_cell(game->map->cells[0][1]);
	char *cmd = "voir";
	int local_test_result = 0;
	//"{sibur phiras thystame, sibur sibur player player, linemate deraumere deraumere deraumere deraumere sibur sibur mendiane phiras}";

	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_inventoire");
	}
	
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
}




void	test_broadcast()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	t_player *player2 = add_player(1, 5);
	

	t_cell *cell = player->curr_cell;
	set_player_cell(player, game->map->cells[0][0]);
	set_player_cell(player2, game->map->cells[0][0]);
	
	print_cell(player->curr_cell);
	
	char *cmd = "broadcast 123";
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	//local_test_result += xassert(player->curr_cell->inventory[NOURRITURE] == cell_nourriture + 1, "no item left from player inv after pose");
	//local_test_result += xassert(player->inventory[NOURRITURE] == 0, "pose: item remains at player inv");
}



void	testcase()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	print_cell(player->curr_cell);
	char *cmd = "prend linemate";
	int local_test_result = 0;
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	
	printf("GO\n");
	print_player(player);
	char *cmd_avance = "avance";
	lgc_execute_command(player_id, cmd_avance, lgc_get_command_id(cmd_avance));

	printf("TURN LEFT\n");
	print_player(player);
	char *cmd_gauche = "gauche";
	lgc_execute_command(player_id, cmd_gauche, lgc_get_command_id(cmd_gauche));

	printf("GO\n");
	print_player(player);
	lgc_execute_command(player_id, cmd_avance, lgc_get_command_id(cmd_avance));


	
	printf("\nCELL BEFORE TAKE\n");
	ft_memset(player->curr_cell->inventory, 0, sizeof(int) * RESOURCES_NUMBER);
	player->curr_cell->inventory[LINEMATE] = 1;
	int player_nourriture = player->inventory[LINEMATE];
	print_cell(player->curr_cell);

	
	printf("\nTAKE LINEMATE\n");
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	print_player(player);


	local_test_result += xassert(player->inventory[LINEMATE] == player_nourriture + 1, "no item in player inv after prend");
	local_test_result += xassert(player->curr_cell->inventory[LINEMATE] == 0, "prend: item remains at cell");
	
}

void	test_set_player_cell()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	print_cell(player->curr_cell);
	char *cmd = "prend linemate";
	int local_test_result = 0;
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	t_cell *cell = player->curr_cell;
	set_player_cell(player, game->map->cells[0][1]);
	print_cell(player->curr_cell);
	print_cell(cell);
}

void	init_game(void)
{
	if (game)
	{
		ft_lstadd(&g_list_game_to_free, ft_lstnew_pointer(game, sizeof(t_game)));
	}
	g_tests_result = 0;
	init_cmd();
	lgc_init();
	game->is_test = 1;
}

int main() {
	game = NULL;
	g_list_game_to_free = (t_list *)ft_memalloc(sizeof(t_list));
	g_messages = (t_dlist *)ft_memalloc(sizeof(t_dlist));
	
	
	//test_avance();
	//test_voir();
	//test_inventoire();
	//test_pose();
	
	test_broadcast();
	//testcase();

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


