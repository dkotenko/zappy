/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:36:19 by gmelisan          #+#    #+#             */
/*   Updated: 2021/11/19 11:12:59 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# define DEBUG

# include "logic/actions_client.h"

# define T_MAX						1000
# define MAX_PENDING_COMMANDS		10

# define CIRCBUF_SIZE				64
# define CIRCBUF_ITEM_SIZE			32

typedef struct s_main_config {
	int		port;
	int		width;
	int		height;
	char	**teams; // pointer to first team in argv
	int		teams_count; // size of `teams'
	int		max_clients;
	int		max_clients_at_team;
	int		t;
	int		quiet;
	int		d;
	t_dict	cmd;
} t_main_config;

extern t_main_config g_cfg;
extern int g_player_count;

/* on program start  */
void srv_start(void);
/* before program exit */
void srv_stop(void);
/* pend message for sending to client */
void srv_reply_client(int client_nb, char *msg, ...) __attribute__ ((format (printf, 2, 3)));
/* instantly send client's writebuf to him and clear the buf */
void srv_flush_client(int client_nb);
/* call when client died by game logic */
void srv_client_died(int client_nb);
/* update timeunit setting */
int srv_update_t(int t);
/* msg will be passed to all gfx clients */
void srv_event(char *msg, ...) __attribute__ ((format (printf, 1, 2)));
/* in result, calls lgc_execute_command(`client_nb', `cmd') after `after_t'  */
void srv_push_command(int client_nb, char *cmd, int after_t);

#endif
