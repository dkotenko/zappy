/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:34:22 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 14:37:25 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H

# include "commands.h"

/* Calls at the very beginning */
void lgc_init(void);

/* Calls on new client connection */
void lgc_new_client(int client_nb, char *team);

/* Calls when client self-disconnected */
void lgc_client_gone(int client_nb);

/* Calls when one time unit elapsed (1/t) */
void lgc_update(void);

/* Calls when command arrives to know when server should call `execute_command' */
int lgc_get_command_duration(char *cmd);

/* Calls when command should be executed */
void lgc_execute_command(int client_nb, char *cmd);


#endif
