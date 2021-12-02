/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 06:46:02 by gmelisan          #+#    #+#             */
/*   Updated: 2021/11/19 10:42:56 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "commands.h"
#include "utils.h"
#include "logger.h"
#include "server.h"

static t_commands commands;

static int g_commands_debug = 0;

/* On push, time should never be the same to not override command in case of
   simultaneous commands.
   */
static int cmpf_push(void *a, void *b)
{
	t_command *c1 = (t_command *)a;
	t_command *c2 = (t_command *)b;

	if (timercmp(&c1->t, &c2->t, >))
		return 1;
	if (timercmp(&c1->t, &c2->t, <))
		return -1;
	++c1->t.tv_usec;
	return 1;
}

static int cmpf(void *a, void *b)
{
	t_command *c1 = (t_command *)a;
	t_command *c2 = (t_command *)b;

	if (timercmp(&c1->t, &c2->t, >))
		return 1;
	if (timercmp(&c1->t, &c2->t, <))
		return -1;
	return 0;
}

static int cmpf_always_true(void *a, void *b)
{
	(void)a;
	(void)b;
	return 0;
}

static void delf(void *v)
{
	t_command *c = (t_command *)v;
	free(c->data);
	free(v);
}

t_command *command_new(struct timeval t, char *data, int client_nb)
{
	t_command *c = (t_command *)malloc(sizeof(t_command));
	xassert(c != NULL, "malloc");
	c->t = t;
	c->data = data;
	c->client_nb = client_nb;
	return c;
}

void command_del(t_command *cmd)
{
	delf(cmd);
}

void commands_push(t_command *cmd)
{
	if (g_commands_debug)
		timerprint(log_debug, &cmd->t, "commands_push");
	commands.tree = ft_btree_avl_insert(commands.tree, cmd, cmpf_push);
}

static t_btree_avl *find_min(t_btree_avl *root)
{
	if (!root)
		return root;
	return (root->left ? find_min(root->left) : root);
}

t_command *commands_min(void)
{
	t_btree_avl *min;

	min = find_min(commands.tree);
	if (g_commands_debug && min)
		timerprint(log_debug, &((t_command *)min->content)->t, "commands_min");
	return (min ? min->content : NULL);
}

void commands_pop(t_command *cmd)
{
	if (g_commands_debug)
		log_debug("commands_pop");
	commands.tree = ft_btree_avl_remove(commands.tree, cmd, cmpf, delf);
}

static t_command **commands_pop_client_poppointer;
static int commands_pop_client_nb;

static void applyf_save_client(void *v)
{
	t_command *c = (t_command *)v;

	if (c->client_nb == commands_pop_client_nb) {
		*commands_pop_client_poppointer = c;
		++commands_pop_client_poppointer;
	}
}

void commands_pop_client(int client_nb)
{
	/* find and save all commands with `client_nb' */
	t_command *commands_to_pop[MAX_PENDING_COMMANDS + 1];
	t_command **ptr = commands_to_pop;

	memset(commands_to_pop, 0, sizeof(commands_to_pop));
	commands_pop_client_poppointer = ptr;
	commands_pop_client_nb = client_nb;
	ft_btree_apply_prefix((t_btree *)commands.tree, applyf_save_client);

	while (*ptr) {
		ft_btree_avl_remove(commands.tree, *ptr, cmpf, delf);
		++ptr;
	}
}

int commands_is_empty(void)
{
	return commands.tree == NULL;
}

void commands_destroy(void)
{
	while (commands.tree) {
		commands.tree = ft_btree_avl_remove(commands.tree, NULL, cmpf_always_true, delf);
	}
}
