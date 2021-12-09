
#include "../zappy.h"
#include "../../server.h"
#include "../../logic.h"
#include "../../utils.h"
#include "../color.h"

void	init_game(void);

static char *default_teams[] = {"team1", "team2", "team3"};
t_main_config g_cfg = {
	.port = 9876,
	.width = 11,
	.height = 12,
	.teams = default_teams,
	.teams_count = 10,
	.max_clients_at_team = 5,
	.t = 1,
	.quiet = 1,
	.d = 0,
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
	local_test_result += xassert(player->curr_cell->x == g_cfg.width - 1 && player->curr_cell->y == g_cfg.height - 1, "avance ORIENT W FAILED");
	
	//must be x=9 y=0
	lgc_execute_command(player_id, "droite", lgc_get_command_id("droite"));
	lgc_execute_command(player_id, "droite", lgc_get_command_id("droite"));
	lgc_execute_command(player_id, "droite", lgc_get_command_id("droite"));
	local_test_result += xassert(player->orient == ORIENT_S, "triple droite FAILED");
	
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	local_test_result += xassert(player->curr_cell->x == g_cfg.width - 1 && player->curr_cell->y == 0, "avance ORIENT S FAILED");

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
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	
	print_player(player);
	ft_memset(player->curr_cell->inventory, 0, sizeof(int) * RESOURCES_NUMBER);
	player->curr_cell->inventory[NOURRITURE] = 1;
	int player_nourriture = player->inventory[NOURRITURE];
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	g_tests_result += xassert(player->inventory[NOURRITURE] == player_nourriture + 1, "no item in player inv after prend");
	g_tests_result += xassert(player->curr_cell->inventory[NOURRITURE] == 0, "prend: item remains at cell");

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
	g_tests_result += xassert(player->curr_cell->inventory[NOURRITURE] == cell_nourriture + 1, "no item left from player inv after pose");
	g_tests_result += xassert(player->inventory[NOURRITURE] == 0, "pose: item remains at player inv");

	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
}


void	test_inventoire()
{
	init_game();
	t_player *player = add_player(3, 5);
	set_player_cell(player, game->map->cells[0][0]);
	int player_id = 3;
	char *cmd = "inventaire";
	
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
	t_player *player = NULL;

	init_game();
	if (game->players_num == 0)
		player = add_player(3, 5);
	
	
	t_player *player2 = add_player(0, 5);
	t_player *player3 = add_player(1, 5);
	set_player_cell(player, game->map->cells[1][1]);

	for (int i = 0; i < game->map->h; i++) {
		for (int j = 0; j < game->map->w; j++) {
			ft_memset(game->map->cells[i][j]->inventory, 0, sizeof(int) * RESOURCES_NUMBER);
		}
	}

	/*
	1S | 1P | 2S
	4P | 0	| 2P
	4S | 3P | 3S
	*/
	int y0 = game->map->h - 1;
	int y1 = 0;
	int y2 = 1;
	int x0 = 0;
	int x1 = 1;
	int x2 = 2;
	

	game->map->cells[y0][x0]->inventory[SIBUR] = 1;
	game->map->cells[y0][x1]->inventory[PHIRAS] = 1;
	game->map->cells[y0][x2]->inventory[SIBUR] = 2;
	game->map->cells[y2][x0]->inventory[SIBUR] = 4;
	game->map->cells[y2][x1]->inventory[PHIRAS] = 3;
	game->map->cells[y2][x2]->inventory[SIBUR] = 3;
	game->map->cells[y1][x0]->inventory[PHIRAS] = 4;
	game->map->cells[y1][x2]->inventory[PHIRAS] = 2;
	player->orient = ORIENT_W;
	
	set_player_cell(player2, game->map->cells[5][5]);
	set_player_cell(player3, game->map->cells[5][5]);
	//print_cell(game->map->cells[0][1]);
	char *cmd = "voir";
	
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
	
	set_player_cell(player, game->map->cells[game->map->h - 1][2]);
	set_player_cell(player2, game->map->cells[1][0]);
	player->orient = ORIENT_N;
	player2->orient = ORIENT_E;


	print_cell(player->curr_cell);
	
	char *cmd = "broadcast 123";
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	lgc_execute_command(player_id, cmd, lgc_get_command_id(cmd));
	//g_tests_result += xassert(player->curr_cell->inventory[NOURRITURE] == cell_nourriture + 1, "no item left from player inv after pose");
	//g_tests_result += xassert(player->inventory[NOURRITURE] == 0, "pose: item remains at player inv");
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
	g_tests_result += local_test_result;
}

void	test_set_player_cell()
{
	t_player *player = NULL;

	init_game();
	if (game->players_num == 0)
		player = add_player(3, 5);
	print_cell(player->curr_cell);
	char *cmd = "prend linemate";
	
	int cmd_id = lgc_get_command_id(cmd);
	if (cmd_id == -1) {
		handle_error("invalid command in test_prend");
	}
	t_cell *cell = player->curr_cell;
	set_player_cell(player, game->map->cells[0][1]);
	print_cell(player->curr_cell);
	print_cell(cell);
}

void	test_player_delete()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	printf("player deleted: %s\n", game->players[player_id] == NULL ? "yes" : "no");
	delete_player(player);
	
	printf("player deleted: %s\n", game->players[player_id] == NULL ? "yes" : "no");
}


void	test_droite_gauche()
{
	t_player *player = NULL;

	init_game();
	int player_id = 3;
	if (game->players_num == 0)
		player = add_player(3, 5);
	
	char *cmd_droite = "droite";
	char *cmd_gauche = "gauche";
	
	int cmd_id_droite = lgc_get_command_id(cmd_droite);
	int cmd_id_gauche = lgc_get_command_id(cmd_gauche);
	if (cmd_id_droite == -1 || cmd_id_gauche == -1) {
		handle_error("invalid command in test_droite_gauche");
	}

	player->orient = ORIENT_N;
	lgc_execute_command(player_id, cmd_droite, lgc_get_command_id(cmd_droite));
	g_tests_result += xassert(player->orient == ORIENT_E, "droite: orientation must be E");
	lgc_execute_command(player_id, cmd_droite, lgc_get_command_id(cmd_droite));
	g_tests_result += xassert(player->orient == ORIENT_S, "droite: orientation must be S");
	lgc_execute_command(player_id, cmd_droite, lgc_get_command_id(cmd_droite));
	g_tests_result += xassert(player->orient == ORIENT_W, "droite: orientation must be W");
	lgc_execute_command(player_id, cmd_droite, lgc_get_command_id(cmd_droite));
	g_tests_result += xassert(player->orient == ORIENT_N, "droite: orientation must be N");

	lgc_execute_command(player_id, cmd_gauche, lgc_get_command_id(cmd_gauche));
	g_tests_result += xassert(player->orient == ORIENT_W, "gauche: orientation must be W");
	lgc_execute_command(player_id, cmd_gauche, lgc_get_command_id(cmd_gauche));
	g_tests_result += xassert(player->orient == ORIENT_S, "gauche: orientation must be S");
	lgc_execute_command(player_id, cmd_gauche, lgc_get_command_id(cmd_gauche));
	g_tests_result += xassert(player->orient == ORIENT_E, "gauche: orientation must be E");
	lgc_execute_command(player_id, cmd_gauche, lgc_get_command_id(cmd_gauche));
	g_tests_result += xassert(player->orient == ORIENT_N, "gauche: orientation must be N");
}

void	init_game(void)
{
	if (game)
	{
		ft_lstadd(&g_list_game_to_free, ft_lstnew_pointer(game, sizeof(t_game)));
	}
	g_tests_result = 0;
	init_cmd();
	lgc_init(10, 1);
}

void	print_node(t_list *node)
{
	if (node == NULL) {
		printf("node: %x\n", node);
	} else {
		printf("node: %x | next: %x | content: %s\n", node, node->next, (char*)(node->content));
	}
}

void	print_list(t_list *list)
{
	printf("list start\n");
	while (list) {
		print_node(list);
		list = list->next;
	}
	printf("list end\n\n");
}

void test_lstpop()
{
	char *s = "a";
	char *ss = "aa";
	char *sss = "aaa";
	char *ssss = "aaaa";
	t_list *node = ft_lstnew(s, strlen(s));
	t_list *node1 = ft_lstnew(ss, strlen(ss));
	t_list *node2 = ft_lstnew(sss, strlen(sss));
	t_list *node3 = ft_lstnew(ssss, strlen(ssss));

	t_list *list = NULL;
	ft_lstadd(&list, node);
	ft_lstadd(&list, node1);
	ft_lstadd(&list, node2);
	ft_lstadd(&list, node3);
	print_list (list);
	ft_lstpop(&list, ssss);
	print_list (list);
	
}

int main() {
	game = NULL;
	g_list_game_to_free = (t_list *)ft_memalloc(sizeof(t_list));
	g_messages = (t_dlist *)ft_memalloc(sizeof(t_dlist));
	
	/*
	test_droite_gauche();
	test_avance();
	test_voir();
	test_inventoire();
	test_pose();
	test_broadcast();
	testcase();
	test_player_delete();
	*/
	test_lstpop();

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


