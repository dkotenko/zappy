/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:36:19 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/30 17:04:19 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# define DEBUG

typedef struct s_main_config {
	int port;
	int world_width;
	int world_height;
	char **teams; // pointer to first team in argv
	int teams_count; // size of `teams'
	int max_clients_at_team;
	int t;
	int quiet;

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
