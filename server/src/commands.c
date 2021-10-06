/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 06:46:02 by gmelisan          #+#    #+#             */
/*   Updated: 2021/10/06 13:25:53 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "commands.h"
#include "utils.h"
#include "logger.h"

static t_commands commands;

static int g_commands_debug = 0;

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
	commands.tree = ft_btree_avl_insert(commands.tree, cmd, cmpf);
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

static int topop_size;
static t_command **topop;
static int topop_i;
static int topop_client_nb;

static void applyf_popnb(void *v)
{
	t_command *c = (t_command *)v;
	if (topop_i >= topop_size)
		return ;

	if (c->client_nb != topop_client_nb)
		return ;
	++topop_i;
	topop[topop_i] = c;
}


/* TODO it's not working */
void commands_popnb(int client_nb, int max_pop)
{
	topop_client_nb = client_nb;
	topop_size = max_pop;
	topop = (t_command **)calloc(max_pop, sizeof(t_command *));
	xassert(topop != NULL, "calloc");
	topop_i = -1;

	ft_btree_apply_prefix((t_btree *)commands.tree, applyf_popnb);

	for (int i = 0; i <= topop_i; ++i)
		ft_btree_avl_remove(commands.tree, topop[i], cmpf, delf);

	free(topop);
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
