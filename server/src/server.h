/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:36:19 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 09:59:33 by gmelisan         ###   ########.fr       */
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
	int teams_count; // teams + teams_count -- pointer to last team
	int clients_number_at_start;
	int t;
	int quiet;

} t_main_config;

extern t_main_config g_main_config;

void srv_start(void);
void srv_reply_client(int client_nb, char *msg);
void srv_client_died(int client_nb);

#endif
