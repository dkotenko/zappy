/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reception.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 14:20:59 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 18:32:44 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEPTION_H
# define RECEPTION_H

/* like in hotel, you know */

enum e_reception_route {
	RECEPTION_ROUTE_NONE,	/* stay still */
	RECEPTION_ROUTE_EXIT,	/* force client to exit (disconnect) */
	RECEPTION_ROUTE_CLIENT, /* client authorized, go to logic */
	RECEPTION_ROUTE_GFX,	/* it's gfx client, switch to `reception_gfx_chat' */
	RECEPTION_ROUTE_ADMIN	/* admin chat */
};

enum e_reception_state {
	RECEPTION_STATE_DEFAULT,
	RECEPTION_STATE_WELCOME,
	RECEPTION_STATE_COUNT
};

typedef struct s_reception {
	int **teams_clients;		/* array with same indices as in g_main_config.teams
										teams_clients[5] - array of clients in 
										g_main_config.teams[5] */
	int *client_states;			/* array of e_reception_state */
} t_reception;

void reception_init(int max_clients);
void reception_clear(void);
void reception_remove_client(int client_nb);
char *reception_find_client_team(int client_nb);

/* Calls in server for all clients by default */
/* Returns e_reception_route */
int reception_chat(int client_nb, char *message);
int reception_gfx_chat(int client_nb, char *message);
int reception_admin_chat(int client_nb, char *message);

#endif
