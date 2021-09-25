/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 06:46:02 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/25 07:29:40 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "utils.h"

static t_commands commands;

static int cmpf(void *a, void *b)
{
	t_command *c1 = (t_command *)a;
	t_command *c2 = (t_command *)b;

	if (timercmp(c1, c2, >))
		return 1;
	if (timercmp(c1, c2, <))
		return -1;
	return 0;
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

void commands_push(t_command *cmd)
{
	commands.tree = ft_btree_avl_insert(commands.tree, cmd, cmpf);
}

static t_btree_avl *find_min(t_btree_avl *root)
{
	return (root->left ? find_min(root->left) : root);
}

t_command *commands_min(void)
{
	t_btree_avl *min;

	min = find_min(commands.tree);
	return (t_command *)min->content;
}

void commands_pop(t_command *cmd)
{
	commands.tree = ft_btree_avl_remove(commands.tree, cmd, cmpf)
}

int commands_empty(void)
{
	return commands.tree == NULL;
}
