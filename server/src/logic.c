/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 20:38:23 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 10:56:23 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "server.h"
#include "logger.h"
#include "logic.h"

void lgc_update(void)
{
	log_tick();
}

void lgc_execute_command(t_command *command)
{
	log_info("execute command '%s' from #%d", command->data, command->client_nb);
	srv_reply_client(command->client_nb, command->data);
	srv_reply_client(command->client_nb, "\nreply ok\n");
}

static int str_starts_with(char *str, char *with)
{
	if (strncmp(str, with, strlen(with)) == 0)
		return 1;
	return 0;
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
