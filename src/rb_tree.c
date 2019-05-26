/*
 * GQRM
 * Copyright (C) 2019-2025 Chaofan Ma <chaofanma@hotmail.com>
 *
 * This file is part of GQRM.
 *
 * GQRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GQRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GQRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rb_tree.h"

/** @enum
 * Identifying the color of a node.
 */
typedef enum {
    RED, BLACK
} rbt_color;

/** @struct
 * Data abstraction for a node in red-black tree.
 * data    - user data stored in this node
 * left    - left child of this node
 * right   - right child of this node
 * parent  - parent of this node
 * height  - 
 * color
 */
typedef struct _rbt_node {
    rbt_data_t           data;
	struct _rbt_node*    left;
	struct _rbt_node*    right;
	struct _rbt_node*    parent;
	size_t               height;
	rbt_color            color;
} rbt_node, *p_rbt_node;

/** @struct
 * Data abstraction for red-black tree.
 * root   - the root 
 * cmp    - function used to compare data stored in nodes
 *          of this red-black tree
 * size   - the number of nodes in this tree
 */
struct _rb_tree{
    p_rbt_node           root;
	rbt_cmp              cmp;
	size_t               size;
};

static rbt_color color(p_rbt_node);
static p_rbt_node create_node(rbt_data_t, rbt_color);
static ds_stat pre_map_subtree(p_rbt_node, rbt_map_func, size_t);
static ds_stat in_map_subtree(p_rbt_node, rbt_map_func, size_t);
static ds_stat post_map_subtree(p_rbt_node, rbt_map_func, size_t);
static ds_stat insert(pt_RBTree, p_rbt_node, rbt_data_t);
static ds_stat insert_fixup(pt_RBTree, p_rbt_node);
static ds_stat left_rotate(p_rbt_node*);
static ds_stat right_rotate(p_rbt_node*);
static ds_stat get_uncle(p_rbt_node, p_rbt_node*);

static rbt_color
color(p_rbt_node nd)
{
    if (!nd)
	    return BLACK;
	return nd->color;
}

static p_rbt_node
create_node(rbt_data_t data, rbt_color color)
{
    p_rbt_node    nd;

    if (!data)
	    return NULL;
	
	if ((nd = malloc(sizeof(rbt_node))) == NULL)
	    return NULL;
	
	nd->data    = data;
	nd->left    = NULL;
	nd->right   = NULL;
	nd->parent  = NULL;
	nd->height  = 1;
	nd->color   = color;
}

pt_RBTree
RBTree_Create(rbt_cmp cmp)
{
    pt_RBTree    rbt;

	if (!cmp)
	    return NULL;
	
	if ((rbt = malloc(sizeof(RBTree))) == NULL)
	    return NULL;
	
	rbt->root  = NULL;
	rbt->cmp   = cmp;
	rbt->size  = 0;

	return rbt;
}

size_t
RBTree_Size(pt_RBTree rb)
{
    if (!rb)
	    return 0;
	return rb->size;
}

ds_stat
RBTree_PreOrderMap(pt_RBTree rbt, rbt_map_func func)
{
    if (!rbt)
	    return DS_ERROR;
	return pre_map_subtree(rbt->root, func, 0);
}

ds_stat
RBTree_InOrderMap(pt_RBTree rbt, rbt_map_func func)
{
    if (!rbt)
	    return DS_ERROR;
	return in_map_subtree(rbt->root, func, 0);
}

ds_stat
RBTree_PostOrderMap(pt_RBTree rbt, rbt_map_func func)
{
    if (!rbt)
	    return DS_ERROR;
	return post_map_subtree(rbt->root, func, 0);
}

static ds_stat
pre_map_subtree(p_rbt_node sub, rbt_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	func(sub->data, depth);
	pre_map_subtree(sub->left, func, depth + 1);
	pre_map_subtree(sub->right, func, depth + 1);
	return DS_OK;
}

static ds_stat
in_map_subtree(p_rbt_node sub, rbt_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	in_map_subtree(sub->left, func, depth + 1);
	if (sub->color == RED)
	    printf("r");
	else
	    printf("b");
	func(sub->data, depth);
	in_map_subtree(sub->right, func, depth + 1);
	return DS_OK;
}

static ds_stat
post_map_subtree(p_rbt_node sub, rbt_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	post_map_subtree(sub->left, func, depth + 1);
	post_map_subtree(sub->right, func, depth + 1);
	func(sub->data, depth);
	return DS_OK;
}

/**
 *TODO
 */
ds_stat
RBTree_Insert(pt_RBTree rb, rbt_data_t data)
{
    if (!rb)
	    return DS_ERROR;
	if (rb->root == NULL) {
	    rb->root = create_node(data, BLACK);
		rb->size++;
		return DS_OK;
	}

	if (insert(rb, rb->root, data) == DS_OK) {
	    rb->size++;
		return DS_OK;
	}
	return DS_ERROR;
}

static ds_stat
insert(pt_RBTree rb, p_rbt_node sub, rbt_data_t data)
{
    p_rbt_node   nd;

    if (!rb || !sub || !data)
	    return DS_ERROR;
	
	while (sub) {
	    if (rb->cmp(data, sub->data) > 0) {
		    if (sub->right) {
			    sub = sub->right;
			} else {
			    if ((nd = create_node(data, RED)) == NULL)
				    return DS_ERROR;
			    sub->right = nd;
				nd->parent = sub;
				insert_fixup(rb, sub->right);
				return DS_OK;
			}
		} else if (rb->cmp(data, sub->data) < 0) {
		    if (sub->left) {
			    sub = sub->left;
			} else {
			    if ((nd = create_node(data, RED)) == NULL)
				    return DS_ERROR;
			    sub->left = nd;
				nd->parent = sub;
				insert_fixup(rb, sub->left);
				return DS_OK;
			}
		} else {
		    return DS_ERROR;
		}
	}
	return DS_ERROR;
}

/*
 *TODO
 */
static ds_stat
insert_fixup(pt_RBTree rb, p_rbt_node sub)
{
    p_rbt_node     uncle;
	p_rbt_node*    rotate_point;

	if (!rb || !sub)
	    return DS_ERROR;
	
	while (rb->root != sub && sub->color == RED && color(sub->parent) == RED) {
	    if (get_uncle(sub, &uncle) == DS_ERROR)
		    return DS_ERROR;
		if (color(uncle) == RED) {
		    sub->parent->color = BLACK;
			uncle->color = BLACK;
			sub->parent->parent->color = RED;
			sub = sub->parent->parent;
		} else {
		    if (sub->parent->parent == rb->root)
			    rotate_point = &rb->root;
			else if (sub->parent->parent->parent->left == sub->parent->parent)
			    rotate_point = &sub->parent->parent->parent->left;
			else if (sub->parent->parent->parent->right == sub->parent->parent)
			    rotate_point = &sub->parent->parent->parent->right;
			else
			    assert(0);

		    /* left cases */
		    if (sub->parent->parent->left == sub->parent) {
			    printf("left ");
			    /* left right case */
			    if (sub->parent->right == sub) {
				    printf("right\n");
				    sub = sub->parent;
				    if (left_rotate(&sub->parent->left) == DS_ERROR)
					    return DS_ERROR;
				}
				/* left left case */
				if (right_rotate(rotate_point) == DS_ERROR)
				    return DS_ERROR;
				sub->parent->color = BLACK;
				sub->parent->right->color = RED;
			/* right cases */
			} else if (sub->parent->parent->right == sub->parent) {
			    printf("right ");
			    /* right left case */
			    if (sub->parent->left == sub) {
				    printf("left\n");
				    sub = sub->parent;
					if (right_rotate(&sub->parent->left) == DS_ERROR)
					    return DS_ERROR;
				}
				/* right right case */
			    if (left_rotate(rotate_point) == DS_ERROR)
				    return DS_ERROR;
				sub->parent->color = BLACK;
				sub->parent->left->color = RED;
			/* left right case */
			}
		}
	}

	rb->root->color = BLACK;

	return DS_OK;
}

/** @fn
 * left rotate a node:
 *        
 *         P             P
 *         |             |
 *         x             y
 *        / \   ---->   / \
 *       &   y         x   ^
 *          / \       / \
 *         #   ^     &   #
 */
static ds_stat
left_rotate(p_rbt_node* sub)
{
    p_rbt_node    right;

	if (!sub || !(*sub) || !(*sub)->right)
	    return DS_ERROR;
	
	right = (*sub)->right;

	(*sub)->right = right->left;
	if (right->left)
	    right->left->parent = *sub;
	right->left = *sub;
	right->parent = (*sub)->parent;
	(*sub)->parent = right;
	*sub = right;

	return DS_OK;
}

/** @fn
 * right rotate a node:
 *
 *            P          P
 *            |          |
 *            y          x
 *           / \  ----> / \
 *          x   ^      &   y
 *         / \            / \
 *        &   #          #   ^
 */
static ds_stat
right_rotate(p_rbt_node* sub)
{
    p_rbt_node  left;

    if (!sub || !(*sub) || !(*sub)->left)
	    return DS_ERROR;
	
	left = (*sub)->left;

	(*sub)->left = left->right;
	if (left->right)
	    left->right->parent = *sub;
	left->right = *sub;
	left->parent = (*sub)->parent;
	(*sub)->parent = left;
	*sub = left;

	return DS_OK;
}

static ds_stat
get_uncle(p_rbt_node sub, p_rbt_node* uncle)
{
    if (!sub || !sub->parent || !sub->parent->parent)
	    return DS_ERROR;
	if (sub->parent->parent->left == sub->parent)
	    *uncle = sub->parent->parent->right;
	else
	    *uncle = sub->parent->parent->left;
	return DS_OK;
}
