/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:36:19 by gmelisan          #+#    #+#             */
/*   Updated: 2021/10/04 20:12:54 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# define DEBUG

# define CMD_NUMBER 12
enum e_command
{
	CMD_AVANCE,
	CMD_DROITE,
	CMD_GAUCHE,
	CMD_VOIR,
	CMD_INVENTAIRE,
	CMD_PREND,
	CMD_POSE,
	CMD_EXPULSE,
	CMD_BROADCAST,
	CMD_INCANTATION,
	CMD_FORK,
	CMD_CONNECT_NBR
};

typedef struct s_cmd_aux {
	int		*durations;
	int		**name;
} t_;

typedef struct s_main_config {
	int		port;
	int		world_width;
	int		world_height;
	char	**teams; // pointer to first team in argv
	int		teams_count; // size of `teams'
	int		max_clients_at_team;
	int		t;
	int		quiet;
	t_dict	cmd;
} t_main_config;

extern t_main_config g_cfg;

void srv_start(void);
void srv_stop(void);
void srv_reply_client(int client_nb, char *msg, ...) __attribute__ ((format (printf, 2, 3)));
void srv_flush_client(int client_nb);
void srv_client_died(int client_nb);
int srv_update_t(int t);
void srv_event(char *msg, ...) __attribute__ ((format (printf, 1, 2)));

#endif
