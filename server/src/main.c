/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:35:26 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/24 18:05:02 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "server.h"
#include "logger.h"

#define USAGE \
	"Usage: %s [-q] [-h] -p <port> -x <width> -y <height> -c <nb> -t <t> -n <team> [<team>] [<team>] ...\n" \
	"  -q quiet mode\n" \
	"  -h print usage\n" \
	"  -p port number\n" \
	"  -x world width\n" \
	"  -y world height\n" \
	"  -c number of clients authorized at the beginning of the game\n" \
	"  -t time unit divider (the greater t is, the faster the game will go)\n" \
	"  -n team_name_1 team_name2 ...\n"

t_main_config g_main_config;

// TODO validate arguments
static void parse_args(int argc, char **argv)
{
	int c;

	while ((c = getopt(argc, argv, "p:x:y:n:c:t:qh")) != -1) {
		switch (c) {
		case 'h':
			printf(USAGE, argv[0]);
			exit(0);
		case 'q':
			g_main_config.quiet = 1;
			break ;
		case 'p':
			g_main_config.port = atoi(optarg);
			break ;
		case 'x':
			g_main_config.world_width = atoi(optarg);
			break ;
		case 'y':
			g_main_config.world_height = atoi(optarg);
			break ;
		case 'c':
			g_main_config.clients_number_at_start = atoi(optarg);
			break ;
		case 't':
			g_main_config.t = atoi(optarg);
			break ;
		case 'n':
			g_main_config.teams = argv + optind - 1;
			g_main_config.teams_count = argc - optind + 1;
			break ;
		case '?':
			exit(1);
		}
	}

}

int main(int argc, char **argv)
{
	parse_args(argc, argv);
	srv_start();
	return 0;
}
