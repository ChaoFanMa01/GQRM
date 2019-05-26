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

#include "avl_tree.h"

/** @struct
 * Data abstraction for a node in AVL tree.
 * data    - the user data stored at this node
 * left    - the left child of this node
 * right   - the right child of this node
 * parent  - the parent of this node
 * height  - the height of this node
 */
typedef struct _avlt_node {
    avlt_data_t           data;
	struct _avlt_node*    left;
	struct _avlt_node*    right;
	struct _avlt_node*    parent;
	size_t                height;
} avlt_node, *p_avlt_node;

/** @struct
 * Data abstraction for an AVL tree.
 * root   - the root of this tree
 * cmp    - the function used to compare data stored in nodes
 * size   - indicating the number of nodes in this tree
 */
typedef struct _avl_tree {
    struct _avlt_node*    root;
	avl_cmp               cmp;
	size_t                size;
}val_tree, p_avl_tree;

static p_avlt_node create_node(avlt_data_t);
static ds_stat pre_map_subtree(p_avlt_node, avlt_map_func, size_t);
static ds_stat in_map_subtree(p_avlt_node, avlt_map_func, size_t);
static ds_stat post_map_subtree(p_avlt_node, avlt_map_func, size_t);
static ds_stat get_max(p_avlt_node, avlt_data_t*);
static ds_stat get_min(p_avlt_node, avlt_data_t*);
static ds_stat insert(pt_AVLTree, p_avlt_node, avlt_data_t);
static ds_stat find_node(p_avlt_node, avlt_data_t, avl_cmp, p_avlt_node*);
static ds_stat find_min_node(p_avlt_node, p_avlt_node*);
static ds_stat delete(pt_AVLTree, p_avlt_node*, avlt_data_t);
static ds_stat right_rotate(p_avlt_node*);
static ds_stat left_rotate(p_avlt_node*);
static ds_stat right_left_rotate(p_avlt_node*);
static ds_stat left_right_rotate(p_avlt_node*);
static size_t height(p_avlt_node);
static ds_stat update_height(p_avlt_node);
static ds_stat balance(p_avlt_node*);
static ds_stat balance_to_root(pt_AVLTree, p_avlt_node);
static void free_subtree(p_avlt_node*);
static p_avlt_node copy_node(p_avlt_node);
static p_avlt_node copy_subtree(p_avlt_node);
static ds_stat in_inter_opt(pt_AVLTree, p_avlt_node, avlt_inter_func);

static p_avlt_node
create_node(avlt_data_t data)
{
    p_avlt_node    nd;
	if (!data)
	    return NULL;

	if ((nd = malloc(sizeof(avlt_node))) == NULL)
	    return NULL;

    nd->data    = data;
	nd->left    = NULL;
	nd->right   = NULL;
	nd->parent  = NULL;
	nd->height  = 1;

	return nd;
}

pt_AVLTree
AVLTree_Create(avl_cmp cmp)
{
    pt_AVLTree    avl;

	if (!cmp)
	    return NULL;
	
	if ((avl = malloc(sizeof(AVLTree))) == NULL)
	    return NULL;
	
	avl->root = NULL;
	avl->cmp  = cmp;
	avl->size = 0;

	return avl;
}

ds_stat
AVLTree_PreOrderMap(pt_AVLTree avl, avlt_map_func func)
{
    if (!avl)
	    return DS_ERROR;
	return pre_map_subtree(avl->root, func, 0);
}

ds_stat
AVLTree_InOrderMap(pt_AVLTree avl, avlt_map_func func)
{
    if (!avl)
	    return DS_ERROR;
	return in_map_subtree(avl->root, func, 0);
}

ds_stat
AVLTree_PostOrderMap(pt_AVLTree avl, avlt_map_func func)
{
    if (!avl)
	    return DS_ERROR;
	return post_map_subtree(avl->root, func, 0);
}

/** @fn
 * For each data in "from", do action denoted by "func"
 * to AVL tree "to".
 */
ds_stat
AVLTree_InterOpt(pt_AVLTree to, pt_AVLTree from, avlt_inter_func func)
{
    if (!to || !from)
	    return DS_ERROR;
	return in_inter_opt(to, from->root, func);
}

/** @fn 
 * Perform some action denoted by "func" for each data in 
 * subtree "sub" to an AVL tree "avl", i.e., for each data
 * in "sub", do operation "func" in AVL tree "avl".
 */
static ds_stat
in_inter_opt(pt_AVLTree avl, p_avlt_node sub, avlt_inter_func func)
{
    if (!avl || !sub || !func)
	    return DS_ERROR;
	
	in_inter_opt(avl, sub, func);
	func(avl, sub->data);
	in_inter_opt(avl, sub, func);
}

static ds_stat
pre_map_subtree(p_avlt_node sub, avlt_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	func(sub->data, depth);
	pre_map_subtree(sub->left, func, depth + 1);
	pre_map_subtree(sub->right, func, depth + 1);
	return DS_OK;
}

static ds_stat
in_map_subtree(p_avlt_node sub, avlt_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	in_map_subtree(sub->left, func, depth + 1);
	func(sub->data, depth);
	in_map_subtree(sub->right, func, depth + 1);
	return DS_OK;
}

static ds_stat
post_map_subtree(p_avlt_node sub, avlt_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	post_map_subtree(sub->left, func, depth + 1);
	post_map_subtree(sub->right, func, depth + 1);
	func(sub->data, depth);
	return DS_OK;
}

ds_stat
AVLTree_GetMax(pt_AVLTree avl, avlt_data_t* re)
{
    if (!avl)
	    return DS_ERROR;
	return get_max(avl->root, re);
}

ds_stat
AVLTree_GetMin(pt_AVLTree avl, avlt_data_t* re)
{
    if (!avl)
	    return DS_ERROR;
    return get_min(avl->root, re);
}

static ds_stat
get_min(p_avlt_node sub, avlt_data_t* re)
{
    if (!sub || !re)
	    return DS_ERROR;
	
	for (; sub->left; sub = sub->left) ;
	*re = sub->data;

	return DS_OK;
}

static ds_stat
get_max(p_avlt_node sub, avlt_data_t* re)
{
    if (!sub || !re)
	    return DS_ERROR;
	
	for (; sub->right; sub = sub->right) ;
	*re = sub->data;
}

size_t
AVLTree_Size(pt_AVLTree avl)
{
    if (!avl)
	    return 0;
	return avl->size;
}

/** @fn 
 * Find the node with given data.
 * NOTE if no nodes contain given data, the DS_ERROR
 * status is returned, otherwise, DS_OK is returned.
 */
static ds_stat
find_node(p_avlt_node sub, avlt_data_t data, avl_cmp cmp, 
          p_avlt_node* re)
{
    if (!sub || !data || !cmp || !re)
	    return DS_ERROR;
	
	while (sub)
	    if (cmp(data, sub->data) == 0) {
		    *re = sub;
			return DS_OK;
		} else if (cmp(data, sub->data) > 0) {
		    sub = sub->right;
		} else {
		    sub = sub->left;
		}
	return DS_ERROR;
}

static ds_stat
find_min_node(p_avlt_node sub, p_avlt_node* re)
{
    if (!sub || !re)
	    return DS_ERROR;
	for (; sub->left; sub = sub->left) ;
	*re = sub;
	return DS_OK;
}

ds_stat
AVLTree_Delete(pt_AVLTree avl, avlt_data_t data)
{
    if (!avl)
	    return DS_ERROR;
	if (delete(avl, &avl->root, data) == DS_OK) {
	    avl->size--;
		return DS_OK;
	}
	return DS_ERROR;
}

static ds_stat
delete(pt_AVLTree avl, p_avlt_node* sub, avlt_data_t data)
{
    p_avlt_node   del = NULL, min = NULL, min_pa;
	p_avlt_node*  point_to_delete;

	if (find_node(*sub, data, avl->cmp, &del) == DS_ERROR)
	    return DS_ERROR;
	
	/* find the address of the pointer which points to
	 * the node to be deleted.
	 */
	/* the node to be deleted is not a root */
	if (del->parent) {
	    /* the node to be deleted is the left child of its parent */
	    if (del == del->parent->left)
		    point_to_delete = &del->parent->left;
	    /* the node to be deleted is the right child of its parent */
		else if (del == del->parent->right)
		    point_to_delete = &del->parent->right;
		/* assert if neither above two cases happen */
		else
		    assert(0);
	/* the node to be deleted is a root */
	} else {
	    point_to_delete = sub;
	}
	
    /** the first case
	 *      O
	 *     / \
	 *   nil  O
	 */
	if (del->left == NULL) {
		*point_to_delete = del->right;
		if (del->right)
		    del->right->parent = del->parent;
		update_height(del->parent);
		balance_to_root(avl, del->parent);
	/** the second case
	 *     O
	 *    / \
	 *   O  nil
	 */
	} else if (del->right == NULL) {
		*point_to_delete = del->left;
		if (del->left)
		    del->left->parent = del->parent;
		update_height(del->parent);
		balance_to_root(avl, del->parent);
	/** the third case
	 *     O
	 *    / \
	 *   O   O  <--- this is the minimum node 
	 *  / \ / \
	 * O  Onil O
	 */
	} else if (del->right->left == NULL) {
	    del->right->left = del->left;
		if (del->left)
    		del->left->parent = del->right;
		*point_to_delete = del->right;
		del->right->parent = del->parent;
		update_height(del->parent);
		balance_to_root(avl, del->parent);
	/** the forth case
	 *           O
	 *          / \
	 *         /   \
	 *        O     O
	 *       ...   /..
	 *      ..... /....
	 *     ......O...... <--- this is the minimum node
	 *    ....../ \......
	 *   ......nil O......
	 */
	} else {
	    find_min_node(del->right, &min);
		assert(min);
		min->parent->left = min->right;
		if (min->right)
		    min->right->parent = min->parent;
		min_pa = min->parent;
		min->left = del->left;
		if (del->left)
		    del->left->parent = min;
		min->right = del->right;
		if (del->right)
		    del->right->parent = min;
		*point_to_delete = min;
		min->parent = del->parent;
		update_height(min_pa);
		balance_to_root(avl, min_pa);
	}

	free(del);
	return DS_OK;
}

size_t
AVLTree_Height(pt_AVLTree bst)
{
    if (!bst)
	    return 0;
	return height(bst->root);
}

static size_t
height(p_avlt_node sub)
{
    if (!sub)
	    return 0;
	return sub->height;
}

static ds_stat
update_height(p_avlt_node sub)
{
    size_t lh, rh;

    if (!sub)
	    return DS_ERROR;
	
    while (sub) {
	    lh = height(sub->left);
    	rh = height(sub->right);
    	sub->height = lh > rh ? lh + 1 : rh + 1;
		sub = sub->parent;
	}
	return DS_OK;
}

ds_stat
AVLTree_Insert(pt_AVLTree avl, avlt_data_t data)
{
    if (!avl)
	    return DS_ERROR;
	if (avl->root == NULL) {
	    avl->root = create_node(data);
		avl->size++;
		return DS_OK;
	}

	if (insert(avl, avl->root, data) == DS_OK) {
	    avl->size++;
		return DS_OK;
	}
	return DS_ERROR;
}

static ds_stat
insert(pt_AVLTree avl, p_avlt_node sub, avlt_data_t data)
{
    p_avlt_node   nd;

    if (!avl || !sub || !data)
	    return DS_ERROR;
	
	while (sub) {
	    if (avl->cmp(data, sub->data) > 0) {
		    if (sub->right) {
			    sub = sub->right;
			} else {
			    if ((nd = create_node(data)) == NULL)
				    return DS_ERROR;
			    sub->right = nd;
				nd->parent = sub;
				update_height(sub);
				balance_to_root(avl, sub);
				return DS_OK;
			}
		} else if (avl->cmp(data, sub->data) < 0) {
		    if (sub->left) {
			    sub = sub->left;
			} else {
			    if ((nd = create_node(data)) == NULL)
				    return DS_ERROR;
			    sub->left = nd;
				nd->parent = sub;
				update_height(sub);
				balance_to_root(avl, sub);
				return DS_OK;
			}
		} else {
		    return DS_ERROR;
		}
	}
	return DS_ERROR;
}

static ds_stat
balance_to_root(pt_AVLTree avl, p_avlt_node sub)
{
    p_avlt_node*   nd;

    if (!avl || !sub)
	    return DS_ERROR;
	while (sub) {
	    if (!sub->parent)
		    nd = &avl->root;
		else if (sub == sub->parent->left)
		    nd = &sub->parent->left;
		else
		    nd = &sub->parent->right;
		balance(nd);

		sub = sub->parent;
	}
	return DS_OK;
}

static ds_stat
balance(p_avlt_node* sub)
{
    int balance_factor, flag = 0;

	if (!sub)
	    return DS_ERROR;
	
	balance_factor = (int)height((*sub)->left) - 
	                 (int)height((*sub)->right);
	if (balance_factor > 1) {
	    flag = 1;
	    /*
		 *     P
		 *     |
		 *     A
		 *    /
		 *   B
		 *    \
		 *     C
		 */
	    if (height((*sub)->left->right) > height((*sub)->left->left))
		    left_rotate(&(*sub)->left);
		right_rotate(sub);
	} else if (balance_factor < -1) {
	    flag = 1;
	    /*
		 *    P
		 *    |
		 *    A
		 *     \
		 *      B
		 *     /
		 *    C
		 */
	    if (height((*sub)->right->left) > height((*sub)->right->right))
		    right_rotate(&(*sub)->right);
		left_rotate(sub);
	}

	if (flag) {
	    update_height((*sub)->left);
	    update_height((*sub)->right);
	}
	return DS_OK;
}

/**
 * right rotation:
 *        P                p
 *        |                |
 *        A                B
 *       / \              / \
 *      B   ^            C   A
 *     / \      --->    / \ / \
 *    C   ^            ^  ^ ^  ^
 *   / \
 *  ^   ^
 */
static ds_stat
right_rotate(p_avlt_node* sub)
{
    p_avlt_node   left;

    if (!sub)
	    return DS_ERROR;
	assert((*sub)->left);

	left                = (*sub)->left;
	left->parent        = (*sub)->parent;
	(*sub)->parent      = left;
	(*sub)->left        = left->right;
	if (left->right)
    	left->right->parent = *sub;
	left->right         = *sub;
	*sub                = left;

	return DS_OK;
}

/**
 * left rotation:
 *     P                  P
 *     |                  |
 *     A                  B
 *    / \                / \
 *   ^   B     ---->    A   C
 *      / \            / \ / \
 *     ^   C          ^  ^ ^  ^
 *        / \
 *       ^   ^
 */
static ds_stat
left_rotate(p_avlt_node* sub)
{
    p_avlt_node   right;
	
	if (!sub)
	    return DS_ERROR;
	assert((*sub)->right);

	right                 = (*sub)->right;
	right->parent         = (*sub)->parent;
	(*sub)->parent        = right;
	(*sub)->right         = right->left;
	if (right->left)
    	right->left->parent   = *sub;
	right->left           = *sub;
	*sub                  = right;

	return DS_OK;
}

/**
 * left-right rotation:
 *         P                       P                     P
 *         |                       |                     |
 *         A                       A                     C
 *        / \   left rotate       / \  right rotate     / \
 *       B   ^  from B           C   ^ from A          B   A
 *      / \     ----------->    / \    ------------>  / \ / \
 *     ^   C                   B   ^                 ^  ^ ^  ^
 *        / \                 / \
 *       ^   ^               ^   ^
 */
static ds_stat
left_right_rotate(p_avlt_node* sub)
{
   if (!sub)
       return DS_ERROR;
   assert((*sub)->left);
   if (left_rotate(&(*sub)->left) == DS_ERROR)
       return DS_ERROR;
   return right_rotate(sub);
}

/**
 * right-left rotation:
 *        P                     P                        P
 *        |                     |                        |
 *        A                     A                        B
 *       / \     right rotate  / \     left rotate      / \
 *      ^   B    from B       ^   B    from A          A   C
 *         / \   ------------>   / \   ----------->   / \ / \
 *        C   ^                 ^   C                ^  ^ ^  ^ 
 *       / \                       / \
 *      ^   ^                     ^   ^
 */
static ds_stat
right_left_rotate(p_avlt_node* sub)
{
    if (!sub)
	    return DS_ERROR;
	assert((*sub)->right);
    if (right_rotate(&(*sub)->right) == DS_ERROR)
	    return DS_ERROR;
	return left_rotate(sub);
}

/** @fn
 * Check whether the given data is contained in this AVL tree.
 */
ds_bool
AVLTree_Contain(pt_AVLTree avl, avlt_data_t data)
{
    p_avlt_node nd;

    if (!avl || !data)
	    return DS_FALSE;

    if (find_node(avl->root, data, avl->cmp, &nd) == DS_ERROR)
	    return DS_FALSE;
	return DS_TRUE;
}

void
AVLTree_Clear(pt_AVLTree avl)
{
    if (!avl)
	    return;
	free_subtree(&avl->root);
	avl->root = NULL;
	avl->size = 0;
}

void
AVLTree_Free(pt_AVLTree* avl)
{
    if (!avl || !(*avl))
	    return;

    free_subtree(&(*avl)->root);
	free((*avl)->root);
	*avl = NULL;
}

static void
free_subtree(p_avlt_node* sub)
{
    if (!sub || !(*sub))
	    return;
	free_subtree(&(*sub)->left);
	free_subtree(&(*sub)->right);
	free(*sub);
	*sub = NULL;
}

pt_AVLTree
AVLTree_Copy(pt_AVLTree avl)
{
    pt_AVLTree cpy;

	if (!avl)
	    return NULL;
	
	if ((cpy = malloc(sizeof(AVLTree))) == NULL)
	    return NULL;
	
	cpy->root = copy_subtree(avl->root);
	cpy->cmp  = avl->cmp;
	cpy->size = avl->size;

	return cpy;
}

static p_avlt_node
copy_subtree(p_avlt_node from)
{
    p_avlt_node  cpy;

    if (!from)
	    return NULL;

    /* copy the root of this subtree */
	if ((cpy = copy_node(from)) == NULL)
	    return NULL;
	/* copy the left subtree of root */
	cpy->left = copy_subtree(from->left);
	if (cpy->left)
	    cpy->left->parent = cpy;
	/* copy the right subtree of root */
	cpy->right = copy_subtree(from->right);
	if (cpy->right)
	    cpy->right->parent = cpy;
	return cpy;
}

static p_avlt_node
copy_node(p_avlt_node nd)
{
    p_avlt_node cpy;

	if (!nd) 
	    return NULL;
	
	if ((cpy = malloc(sizeof(avlt_node))) == NULL)
	    return NULL;
	
	cpy->data   = nd->data;
	cpy->height = nd->height;
	cpy->left   = NULL;
	cpy->right  = NULL;
	cpy->parent = NULL;

	return cpy;
}
