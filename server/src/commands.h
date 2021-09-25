/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 06:29:14 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/25 21:49:44 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
# define COMMANDS_H

# include <sys/time.h>

# include "ft_btree.h"

typedef struct s_command {
	struct timeval t;
	char *data;
	int client_nb;
} t_command;

typedef struct s_commands {
	t_btree_avl *tree;
} t_commands;

/* allocates structure */
t_command *command_new(struct timeval t, char *data, int client_nb);
void command_del(t_command *cmd);

/* cmd must be malloc'd */
void commands_push(t_command *cmd);
t_command *commands_min(void);
void commands_pop(t_command *cmd);
int commands_empty(void);

#endif
