/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 11:48:08 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/29 15:38:30 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "graphic.h"
#include "server.h"
#include "logic.h"
#include "logger.h"

static void error(int client_nb)
{
	srv_reply_client(client_nb, "sbp\n");
}

static void msz(int client_nb)
{
	srv_reply_client(client_nb, "msz %d %d\n",
					 g_cfg.width, g_cfg.height);
}

static void bct(int client_nb)
{
	int resources[7];
	int x;
	int y;
	char *token = strtok(NULL, " ");
	if (!token) {
		error(client_nb);
		return ;
	}
	x = atoi(token);
	token = strtok(NULL, " ");
	if (!token) {
		error(client_nb);
		return ;
	}
	y = atoi(token);
	if (lgc_get_cell_resources(x, y, resources) == -1) {
		error(client_nb);
		return ;
	}
	srv_reply_client(client_nb, "bct %d %d %d %d %d %d %d %d %d\n",
					 x, y, resources[0], resources[1], resources[2],
					 resources[3], resources[4], resources[5], resources[6]);
}

static void mct(int client_nb)
{
	int resources[7];
	
	for (int i = 0; i < g_cfg.height; ++i) {
		for (int j = 0; j < g_cfg.width; ++j) {
			if (lgc_get_cell_resources(i, j, resources) == 1)
				srv_reply_client(client_nb, "bct %d %d %d %d %d %d %d %d %d\n",
								 i, j, resources[0], resources[1], resources[2],
								 resources[3], resources[4], resources[5], resources[6]);
		}
		srv_flush_client(client_nb);
	}
}

static void tna(int client_nb)
{
	int i;
	for (i = 0; i < g_cfg.teams_count; ++i)
		srv_reply_client(client_nb, "tna %s\n", g_cfg.teams[i]);
}

static void ppo(int client_nb)
{
	int x;
	int y;
	int o;
	
	char *token = strtok(NULL, " ");
	if (!token) {
		error(client_nb);
		return ;
	}
	int n = atoi(token);

	if (lgc_get_player_position(n, &x, &y, &o) != 0) {
		error(client_nb);
		return ;
	}
	srv_reply_client(client_nb, "ppo %d %d %d %d\n", n, x, y, o);
}

static void plv(int client_nb)
{
	char *token = strtok(NULL, " ");
	if (!token) {
		error(client_nb);
		return ;
	}
	int n = atoi(token);
	int level;
	if ((level = lgc_get_player_level(n)) < 0) {
		error(client_nb);
		return ;
	}
	srv_reply_client(client_nb, "plv %d %d\n", n, level);
}

static void pin(int client_nb)
{
	int x;
	int y;
	int resources[7];

	char *token = strtok(NULL, " ");
	if (!token) {
		error(client_nb);
		return ;
	}
	int n = atoi(token);
	if (lgc_get_player_inventory(n, &x, &y, resources) != 0) {
		error(client_nb);
		return ;
	}
	srv_reply_client(client_nb, "pin %d %d %d %d %d %d %d %d %d %d\n",
					 n, x, y, resources[0], resources[1], resources[2],
					 resources[3], resources[4], resources[5], resources[6]);
}

static void sgt(int client_nb)
{
	srv_reply_client(client_nb, "sgt %d\n", g_cfg.t);
}

static void sst(int client_nb)
{
	int t;

	char *token = strtok(NULL, " ");
	if (!token) {
		error(client_nb);
		return ;
	}
	t = atoi(token);
	if ((t = srv_update_t(t)) == -1) {
		error(client_nb);
		return ;
	}
	srv_reply_client(client_nb, "sgt %d\n", t);
}

int graphic_chat(int client_nb, char *message)
{
	if (!message) {
		msz(client_nb);
		sgt(client_nb);
		mct(client_nb);
		tna(client_nb);
		// pnw
		// enw
		return 0;
	}
	char *token = strtok(message, " ");
	if (!token)
		return 0;
	
	if (strcmp(token, "msz") == 0)
		msz(client_nb);
	else if (strcmp(token, "bct") == 0)
		bct(client_nb);
	else if (strcmp(token, "mct") == 0)
		mct(client_nb);
	else if (strcmp(token, "tna") == 0)
		tna(client_nb);
	else if (strcmp(token, "ppo") == 0)
		ppo(client_nb);
	else if (strcmp(token, "plv") == 0)
		plv(client_nb);
	else if (strcmp(token, "pin") == 0)
		pin(client_nb);
	else if (strcmp(token, "sgt") == 0)
		sgt(client_nb);
	else if (strcmp(token, "sst") == 0)
		sst(client_nb);
	else
		srv_reply_client(client_nb, "suc\n");
	return 0;
}
