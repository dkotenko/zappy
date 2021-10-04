/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:35:26 by gmelisan          #+#    #+#             */
/*   Updated: 2021/10/04 20:11:51 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "server.h"
#include "logger.h"

static char *default_teams[] = {"team1", "team2", "team3"};

#define USAGE \
	"Usage: %s [-q] [-h] -p <port> -x <width> -y <height> -c <nb> -t <t> -n <team> [<team>] [<team>] ...\n" \
	"  -q quiet mode\n" \
	"  -h print usage\n" \
	"  -p port number, default 9876\n" \
	"  -x world width\n" \
	"  -y world height\n" \
	"  -c number of clients authorized at the team\n" \
	"  -t time unit divider (the greater t is, the faster the game will go), default 1\n" \
	"  -n team_name_1 team_name_2 ...\n"

t_main_config g_cfg = {
	.port = 9876,
	.world_width = 20,
	.world_height = 20,
	.teams = default_teams,
	.teams_count = 3,
	.max_clients_at_team = 5,
	.t = 1,
	.quiet = 0,
	.cmd = NULL
};

static void validate_arg(int cond, const char *message)
{
	if (!cond) {
		log_error("%s", message);
		exit(1);
	}
}

static void validate_args(void)
{
	validate_arg(g_cfg.port > 0 && g_cfg.port < 65536,
				 "Port is out of range [1, 65535]");
	validate_arg(g_cfg.world_width > 0,
				 "World width must be positive");
	validate_arg(g_cfg.world_height > 0,
				 "World height must be positive");
	for (int i = 0; i < g_cfg.teams_count; ++i)
		validate_arg(g_cfg.teams[i][0] != 0, "Team name must not be empty");
	validate_arg(g_cfg.max_clients_at_team > 0,
				 "Number of clients in team must be positive");
	/* g_cfg.t will be checked in srv_create() */
}

static void parse_args(int argc, char **argv)
{
	int c;

	while ((c = getopt(argc, argv, "p:x:y:n:c:t:qh")) != -1) {
		switch (c) {
		case 'h':
			printf(USAGE, argv[0]);
			exit(0);
		case 'q':
			g_cfg.quiet = 1;
			break ;
		case 'p':
			g_cfg.port = atoi(optarg);
			break ;
		case 'x':
			g_cfg.world_width = atoi(optarg);
			break ;
		case 'y':
			g_cfg.world_height = atoi(optarg);
			break ;
		case 'c':
			g_cfg.max_clients_at_team = atoi(optarg);
			break ;
		case 't':
			g_cfg.t = atoi(optarg);
			break ;
		case 'n':
			g_cfg.teams = argv + optind - 1;
			g_cfg.teams_count = argc - optind + 1;
			break ;
		case '?':
			exit(1);
		}
	}

}

void	init_cmd()
{
	g_cfg.cmd.durations = (int *)malloc(sizeof(int) * CMD_NUMBER);
	
}

int main(int argc, char **argv)
{
	init_cmd(); 
	parse_args(argc, argv);
	validate_args();
	srv_start();
	return 0;
}
