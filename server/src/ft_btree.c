/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 06:23:11 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/25 06:26:43 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_btree.h"

void	ft_btree_apply_prefix(t_btree *root, void (*applyf)(void *))
{
	if (root)
	{
		applyf(root->content);
		ft_btree_apply_prefix(root->left, applyf);
		ft_btree_apply_prefix(root->right, applyf);
	}
}

void	ft_btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (root)
	{
		ft_btree_apply_infix(root->left, applyf);
		applyf(root->content);
		ft_btree_apply_infix(root->right, applyf);
	}
}

void	ft_btree_apply_suffix(t_btree *root, void (*applyf)(void *))
{
	if (root)
	{
		ft_btree_apply_suffix(root->left, applyf);
		ft_btree_apply_suffix(root->right, applyf);
		applyf(root->content);
	}
}

t_btree		*ft_btree_search(t_btree *root,
							void *content,
							int (*cmpf)(void *, void *))
{
	t_btree	*ret;

	if (root)
	{
		if (root->left)
		{
			ret = ft_btree_search(root->left, content, cmpf);
			if (ret)
				return (ret);
		}
		if ((*cmpf)(root->content, content) == 0)
			return (root);
		if (root->right)
		{
			ret = ft_btree_search(root->right, content, cmpf);
			if (ret)
				return (ret);
		}
	}
	return (NULL);
}


t_btree_avl		 *ft_btree_avl_create_node(void *content)
{
	t_btree_avl *node;

	node = (t_btree_avl *)malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->content = content;
	return (node);
}

static void			fix_height(t_btree_avl *root)
{
	t_uchar	hl;
	t_uchar hr;

	hl = root->left ? root->left->height : 0;
	hr = root->right ? root->right->height : 0;
	root->height = hl > hr ? hl : hr;
	root->height++;
}

static int			bfactor(t_btree_avl *root)
{
	t_uchar	hl;
	t_uchar hr;

	hl = root->left ? root->left->height : 0;
	hr = root->right ? root->right->height : 0;
	return ((int)(hr - hl));
}

static t_btree_avl	*rotate_left(t_btree_avl *root)
{
	t_btree_avl *newroot;

	newroot = root->right;
	root->right = newroot->left;
	newroot->left = root;
	fix_height(root);
	fix_height(newroot);
	return (newroot);
}

static t_btree_avl	*rotate_right(t_btree_avl *root)
{
	t_btree_avl *newroot;

	newroot = root->left;
	root->left = newroot->right;
	newroot->right = root;
	fix_height(root);
	fix_height(newroot);
	return (newroot);
}

t_btree_avl		*ft_btree_avl_balance(t_btree_avl *root)
{
	fix_height(root);
	if (bfactor(root) == 2)
	{
		if (bfactor(root->right) < 0)
			root->right = rotate_right(root->right);
		return (rotate_left(root));
	}
	if (bfactor(root) == -2)
	{
		if (bfactor(root->left) > 0)
			root->left = rotate_left(root->left);
		return (rotate_right(root));
	}
	return (root);
}


t_btree_avl			*ft_btree_avl_insert(t_btree_avl *root,
										void *content,
										int (*cmpf)(void *, void *))
{
	if (!root)
		return (ft_btree_avl_create_node(content));
	if (cmpf(content, root->content) < 0)
		root->left = ft_btree_avl_insert(root->left, content, cmpf);
	else
		root->right = ft_btree_avl_insert(root->right, content, cmpf);
	return (ft_btree_avl_balance(root));
}

static t_btree_avl	*find_min(t_btree_avl *root)
{
	return (root->left ? find_min(root->left) : root);
}

static t_btree_avl	*remove_min(t_btree_avl *root)
{
	if (!root->left)
		return (root->right);
	root->left = remove_min(root->left);
	return (ft_btree_avl_balance(root));
}

t_btree_avl			*ft_btree_avl_remove(t_btree_avl *root,
										void *content,
										int (*cmpf)(void *, void *))
{
	t_btree_avl *left;
	t_btree_avl *right;
	t_btree_avl *min;

	if (!root)
		return (NULL);
	if (cmpf(content, root->content) < 0)
		root->left = ft_btree_avl_remove(root->left, content, cmpf);
	else if (cmpf(content, root->content) > 0)
		root->right = ft_btree_avl_remove(root->right, content, cmpf);
	else
	{
		left = root->left;
		right = root->right;
		free(root->content);
		free(root);
		if (!right)
			return (left);
		min = find_min(right);
		min->right = remove_min(right);
		min->left = left;
		return (ft_btree_avl_balance(min));
	}
	return (ft_btree_avl_balance(root));
}
