/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 20:38:23 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/30 19:54:31 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>

#include "server.h"
#include "logger.h"
#include "logic.h"
#include "utils.h"
#include "reception.h"

/*

void lgc_init(void)
{
	log_info("logic: Setup world");
}

void lgc_new_player(int client_nb, char *team)
{
	log_info("logic: Add player #%d (team '%s')", client_nb, team);
	int x = 0;
	int y = 0;
	int o = 2;
	int l = 1;
	srv_event("pnw %d %d %d %d %d %s\n", client_nb, x, y, o, l, team);
}

void lgc_player_gone(int client_nb)
{
	log_info("logic: Remove client #%d", client_nb);
}

void lgc_update(void)
{
	usleep(100);
}

void lgc_execute_command(int client_nb, char *cmd)
{
	if (strcmp(cmd, "connect_nbr") == 0) {
		// TODO get team name from logic, not from reception (search is long)
		srv_reply_client(client_nb, "%d\n",
						 reception_slots_in_team(reception_find_client_team(client_nb)));
	} else {
		log_info("logic: Execute command '%s' from #%d", cmd, client_nb);
		srv_reply_client(client_nb, "%s - ok\n", cmd);
	}
}



int lgc_get_cell_resources(int x, int y, int resources[7])
{
	if (x < 0 || y < 0)
		return -1;
	for (int i = 0; i < 7; ++i)
		resources[i] = i;
	return 1;
}

int lgc_get_player_position(int client_nb, int *x, int *y, int *o)
{
	if (client_nb < 0)
		return -1;
	*x = 12;
	*y = 34;
	*o = 2;
	return 0;
}

int lgc_get_player_level(int client_nb)
{
	if (client_nb < 0)
		return -1;
	return 1;
}

int lgc_get_player_inventory(int client_nb, int *x, int *y, int resources[7])
{
	if (client_nb < 0)
		return -1;
	*x = 34;
	*y = 56;

	for (int i = 0; i < 7; ++i)
		resources[i] = i + 1;
	return 0;
}


*/
