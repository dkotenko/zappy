/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 20:38:23 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 18:51:16 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>

#include "server.h"
#include "logger.h"
#include "logic.h"
#include "utils.h"

void lgc_init(void)
{
	log_info("logic: Setup world");
}

void lgc_new_client(int client_nb, char *team)
{
	log_info("logic: Add client #%d (team '%s')", client_nb, team);
}

void lgc_client_gone(int client_nb)
{
	log_info("logic: Remove client #%d", client_nb);
}

void lgc_update(void)
{
	usleep(100);
}

void lgc_execute_command(int client_nb, char *cmd)
{
	log_info("logic: Execute command '%s' from #%d", cmd, client_nb);
	srv_reply_client(client_nb, "%s - ok\n", cmd);
}

int lgc_get_command_duration(char *cmd)
{
	if (str_starts_with(cmd, "avance"))
		return 7;
	if (str_starts_with(cmd, "droite"))
		return 7;
	if (str_starts_with(cmd, "gauche"))
		return 7;
	if (str_starts_with(cmd, "voir"))
		return 7;
	if (str_starts_with(cmd, "inventaire"))
		return 1;
	if (str_starts_with(cmd, "prend"))
		return 7;
	if (str_starts_with(cmd, "pose"))
		return 7;
	if (str_starts_with(cmd, "expulse"))
		return 7;
	if (str_starts_with(cmd, "broadcast"))
		return 7;
	if (str_starts_with(cmd, "incantation"))
		return 300;
	if (str_starts_with(cmd, "fork"))
		return 42;
	if (str_starts_with(cmd, "connect_nbr"))
		return 0;
	return -1;
}
