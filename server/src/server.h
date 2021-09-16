/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 16:36:19 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/16 18:13:15 by gmelisan         ###   ########.fr       */
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

#endif
