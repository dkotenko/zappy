/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reception.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 15:10:18 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/30 17:43:40 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "reception.h"
#include "utils.h"
#include "logger.h"
#include "graphic.h"
#include "admin.h"

static t_reception reception;

void reception_init(int max_clients)
{
	xassert((reception.teams_clients =
			(int **)calloc(g_cfg.teams_count, sizeof(int *))) != NULL,
			"calloc");
	xassert((reception.client_states =
			 (int *)calloc(max_clients, sizeof(int))) != NULL,
			"calloc");
}

void reception_clear(void)
{
	for (int i = 0; i < g_cfg.teams_count; ++i) {
		free(reception.teams_clients[i]);
	}
	free(reception.teams_clients);
	free(reception.client_states);
}

/* TODO in fact fd=0 is valid fd, but who cares */

void reception_remove_client(int client_nb)
{
	for (int i = 0; i < g_cfg.teams_count; ++i) {
		for (int j = 0; j < g_cfg.max_clients_at_team; ++j) {
			if (reception.teams_clients[i] && reception.teams_clients[i][j] == client_nb)
				reception.teams_clients[i][j] = 0;
		}
	}
	reception.client_states[client_nb] = 0;
}

char *reception_find_client_team(int client_nb)
{
	for (int i = 0; i < g_cfg.teams_count; ++i) {
		for (int j = 0; j < g_cfg.max_clients_at_team; ++j) {
			if (reception.teams_clients[i] && reception.teams_clients[i][j] == client_nb)
				return g_cfg.teams[i];
		}
	}
	return NULL;
}

static void add_client_to_team(int team_index, int client_nb)
{
	int i = 0;
	
	if (!reception.teams_clients[team_index])
		xassert((reception.teams_clients[team_index] =
				 (int *)calloc(g_cfg.max_clients_at_team + 1,
							   sizeof(int))) != NULL, "calloc");
	while (reception.teams_clients[team_index][i])
		++i;
	if (i < g_cfg.max_clients_at_team)
		reception.teams_clients[team_index][i] = client_nb;
	else
		log_error("can't add client to team");
}

static int count_clients_in_team(int team_index)
{
	int count = 0;
	for (int i = 0; i < g_cfg.max_clients_at_team; ++i) {
		if (reception.teams_clients[team_index]
			&& reception.teams_clients[team_index][i])
			++count;
	}
	return count;
}

static int get_team_index(const char *team)
{
	for (int i = 0; i < g_cfg.teams_count; ++i) {
		if (!g_cfg.teams[i])
			return 0;
		if (strcmp(g_cfg.teams[i], team) == 0)
			return i;
	}
	return -1;
}

int reception_slots_in_team(char *team)
{
	int team_index = get_team_index(team);
	if (team_index == -1)
		return -1;
	int clients_in_team = count_clients_in_team(team_index);
	return g_cfg.max_clients_at_team - clients_in_team;
}

int reception_chat(int client_nb, char *message)
{
	int clients_in_team = 0;
	int ret_nb_client;
	int team_index;
	
	switch (reception.client_states[client_nb]) {
	case RECEPTION_STATE_DEFAULT:
		reception.client_states[client_nb] = RECEPTION_STATE_WELCOME;
		srv_reply_client(client_nb, "BIENVENUE\n");
		return RECEPTION_ROUTE_NONE;
	case RECEPTION_STATE_WELCOME:
		if (strcmp(message, "GRAPHIC") == 0) {
			graphic_chat(client_nb, NULL);
			return RECEPTION_ROUTE_GFX;
		}
		if (strcmp(message, "ADMIN") == 0) {
			admin_chat(client_nb, NULL);
			return RECEPTION_ROUTE_ADMIN;
		}
		if ((team_index = get_team_index(message)) == -1) {
			reception.client_states[client_nb] = 0;
			srv_reply_client(client_nb, "Invalid team name\n");
			return RECEPTION_ROUTE_EXIT;
		}
		clients_in_team = count_clients_in_team(team_index);
		if (clients_in_team >= g_cfg.max_clients_at_team) {
			srv_reply_client(client_nb, "Team is full\n");
			reception.client_states[client_nb] = 0;
			return RECEPTION_ROUTE_EXIT;
		}
		add_client_to_team(team_index, client_nb);
		ret_nb_client = g_cfg.max_clients_at_team - clients_in_team - 1;
		srv_reply_client(client_nb, "%d\n", ret_nb_client);
		srv_reply_client(client_nb, "%d %d\n",
						 g_cfg.world_width, g_cfg.world_height);
		return RECEPTION_ROUTE_CLIENT;
	}
	return RECEPTION_ROUTE_NONE;
}
