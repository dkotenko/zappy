/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:34:22 by gmelisan          #+#    #+#             */
/*   Updated: 2021/10/04 19:45:28 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H

# include "commands.h"

/* Calls at the very beginning */
void lgc_init(void);

/* Calls on new client connection */
void lgc_new_player(int client_nb, char *team);

/* Calls when client self-disconnected */
void lgc_player_gone(int client_nb);

/* Calls when one time unit elapsed (1/t) */
void lgc_update(void);

/* Calls when command arrives to know when server should call `execute_command' */
int lgc_get_command_duration(char *cmd);

/* Calls when command arrives to know command id */
int lgc_get_command_id(char *cmd);

/* Calls when command should be executed */
void lgc_execute_command(int client_nb, char *cmd);

/* store resources of cell (x,y) to provided array 
0 - nourriture, 1 - linemate, 2 - deraumere, 3 - sibur, 
4 - mendiane, 5 - phiras, 6 - thystame

Returns 1 if good, 0 if no resources, -1 if invalid coordinates 
If no resources, array should be zeroed anyway */
int lgc_get_cell_resources(int x, int y, int resources[7]);

/* returns in arguments coordinates of player and his orientation 
Returns 0 if good, -1 if wrong client_nb
*/
int lgc_get_player_position(int client_nb, int *x, int *y, int *o);

/* Returns level of player or -1 if wrong client_nb */
int lgc_get_player_level(int client_nb);

/* Get player position and inventory 
Returns 0 if good, -1 if wrong client_nb*/
int lgc_get_player_inventory(int client_nb, int *x, int *y, int resources[7]);


#endif
