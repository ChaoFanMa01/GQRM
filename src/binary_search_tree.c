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

#include "binary_search_tree.h"

/* @struct 
 * Data type abstraction for a node in binary search tree.
 * data   - data stored at this node
 * left   - the left child of this node
 * right  - the right child of this node
 */
typedef struct _bst_node {
    bst_data_t          data;
	struct _bst_node*   left;
	struct _bst_node*   right;
} bst_node, *p_bst_node;

/* @struct
 * Data type abstraction for a binary search tree.
 * root  - the root of this tree
 * cmp   - a function used to compare two data stored in nodes
 * size  - indicating the number of nodes in this tree
 */
typedef struct _binary_search_tree {
    bst_node*    root;
	bst_cmp      cmp;
	size_t       size;
} bstree, *p_bstree;

static ds_stat pre_map_subtree(p_bst_node, bst_map_func, size_t);
static ds_stat in_map_subtree(p_bst_node, bst_map_func, size_t);
static ds_stat post_map_subtree(p_bst_node, bst_map_func, size_t);
static ds_stat get_max(p_bst_node, bst_data_t*);
static ds_stat get_min(p_bst_node, bst_data_t*);
static p_bst_node create_node(bst_data_t);
static ds_stat find_parent(p_bst_node, bst_data_t, bst_cmp, p_bst_node*);
static ds_stat insert(p_bst_node, bst_data_t, bst_cmp);
static ds_stat get_min_parent(p_bst_node, p_bst_node*);

p_bstree
BSTree_Create(bst_cmp cmp)
{
    p_bstree    bst;

	if (!cmp)
	    return NULL;
	if ((bst = malloc(sizeof(bstree))) == NULL)
	    return NULL;
	
	bst->root = NULL;
	bst->cmp  = cmp;
	bst->size = 0;
	
	return bst;
}

ds_stat
BSTree_PreOrderMap(p_bstree bst, bst_map_func func)
{
    if (!bst)
	    return DS_ERROR;

    return pre_map_subtree(bst->root, func, 0);
}

ds_stat
BSTree_InOrderMap(p_bstree bst, bst_map_func func)
{
    if (!bst)
	    return DS_ERROR;
	
	return in_map_subtree(bst->root, func, 0);
}

ds_stat
BSTree_PostOrderMap(p_bstree bst, bst_map_func func)
{
    if (!bst)
	    return DS_ERROR;
	
	return post_map_subtree(bst->root, func, 0);
}

static ds_stat
pre_map_subtree(p_bst_node sub, bst_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	func(sub->data, depth);
	pre_map_subtree(sub->left, func, depth + 1);
	pre_map_subtree(sub->right, func, depth + 1);

	return DS_OK;
}

static ds_stat
in_map_subtree(p_bst_node sub, bst_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	in_map_subtree(sub->left, func, depth + 1);
	func(sub->data, depth);
	in_map_subtree(sub->right, func, depth + 1);

	return DS_OK;
}

static ds_stat
post_map_subtree(p_bst_node sub, bst_map_func func, size_t depth)
{
    if (!sub || !func)
	    return DS_ERROR;
	
	post_map_subtree(sub->left, func, depth + 1);
	post_map_subtree(sub->right, func, depth + 1);
	func(sub->data, depth);

	return DS_OK;
}

ds_stat
BSTree_GetMax(p_bstree bst, bst_data_t* re)
{
    if (!bst)
	    return DS_ERROR;
	return get_max(bst->root, re);
}

ds_stat
BSTree_GetMin(p_bstree bst, bst_data_t* re)
{
    if (!bst)
	    return DS_ERROR;
	return get_min(bst->root, re);
}

static ds_stat
get_min(p_bst_node sub, bst_data_t* re)
{
    if (!sub || !re)
	    return DS_ERROR;
	for (; sub->left; sub = sub->left) ;
	*re = sub->data;

	return DS_OK;
}

static p_bst_node
create_node(bst_data_t data)
{
    p_bst_node nd = malloc(sizeof(bst_node));
	if (!nd)
	    return NULL;
	nd->data  = data;
	nd->left  = NULL;
	nd->right = NULL;

	return nd;
}

/* @fn
 * Find the parent node of the node containing given data,
 * and return the parent node through parameter "re".
 * It's worth NOTING that when the root stores given data,
 * "re" is set to NULL and return DS_OK; When NOT FIND given
 * data in this subtree, this function will return DS_ERROR.
 * That's to say when return is DS_ERROR, the data in "re"
 * is INVALID!!!
 */
static ds_stat
find_parent(p_bst_node sub, bst_data_t data, bst_cmp cmp, p_bst_node* re)
{
    if (!cmp || !sub || !data || !re)
	    return DS_ERROR;

    if (cmp(sub->data, data) == 0) {
	    *re = NULL;
		return DS_OK;
	}
    while (sub) {
	    if (cmp(data, sub->data) > 0) {
		    if (sub->right) {
			    printf("check right\n");
		        if (cmp(sub->right->data, data) == 0) {
				    printf("find\n");
			        *re = sub;
			    	return DS_OK;
			    } else {
				    printf("go to right\n");
				    sub = sub->right;
				}
			} else {
			    return DS_ERROR;
			}
		} else if (cmp(data, sub->data) < 0) {
		    if (sub->left) {
			    printf("check left\n");
			    if (cmp(sub->left->data, data) == 0) {
				    printf("find\n");
				    *re = sub;
					return DS_OK;
				} else {
				    printf("go to left\n");
				    sub = sub->left;
				}
			} else {
			    return DS_ERROR;
			}
		} else {
		    return DS_ERROR;
		}
	}
	return DS_ERROR;
}

static ds_stat
get_max(p_bst_node sub, bst_data_t* re)
{
    if (!sub || !re)
	    return DS_ERROR;
	for (; sub->right; sub = sub->right) ;
	*re = sub->data;

	return DS_OK;
}

/* @fn
 * Find the parent of the minimun node.
 * If the given root of the subtree is the minimum node,
 * p will set to NULL, and return DS_OK.
 */
static ds_stat
get_min_parent(p_bst_node sub, p_bst_node* p)
{
    if (!sub || !p)
	    return DS_ERROR;
	if (sub->left == NULL) {
	    *p = NULL;
		return DS_OK;
	}
	for (; sub->left->left; sub = sub->left) ;
	*p = sub;
	return DS_OK;
}

ds_stat
BSTree_Delete(p_bstree bst, bst_data_t data)
{
    p_bst_node   pa = NULL, pa_min = NULL, del = NULL, min = NULL;

	if (!bst || !data)
	    return DS_ERROR;
	
	if (find_parent(bst->root, data, bst->cmp, &pa) == DS_ERROR)
	    return DS_ERROR;
	
	if (!pa) {
	    if (bst->root->left == NULL) {
		    del = bst->root;
		    bst->root = bst->root->right;
			free(del);
			bst->size--;
		} else if (bst->root->right == NULL) {
		    del = bst->root;
		    bst->root = bst->root->left;
			free(del);
			bst->size--;
		} else if (bst->root->right->left == NULL) {
		    del = bst->root;
			bst->root->right->left = bst->root->left;
			bst->root = bst->root->right;
			free(del);
			bst->size--;
		} else {
		    get_min_parent(bst->root->right, &pa_min);
			assert(pa_min);
			min = pa_min->left;
			assert(min);
			pa_min->left = min->right;
			del = bst->root;
			min->left = bst->root->left;
			min->right = bst->root->right;
			bst->root = min;
			free(del);
			bst->size--;
		}
	} else {
	    if (pa->left)
		    if (bst->cmp(pa->left->data, data) == 0)
			    del = pa->left;
		if (pa->right)
		    if (bst->cmp(pa->right->data, data) == 0)
			    del = pa->right;
		assert(del);
	    if (del->left == NULL) {
		    if (pa->left == del)
			    pa->left = del->right;
			else if (pa->right == del)
			    pa->right = del->right;
			else
			    assert(0);
			free(del);
			bst->size--;
		} else if (del->right == NULL) {
		    if (pa->left == del)
			    pa->left = del->left;
			else if (pa->right == del)
			    pa->right = del->left;
			else
			    assert(0);
			free(del);
			bst->size--;
		} else if (del->right->left == NULL) {
		    if (pa->left == del) {
			    del->right->left = del->left;
				pa->left = del->right;
				free(del);
				bst->size--;
			} else if (pa->right == del) {
			    del->right->left = del->left;
				pa->right = del->right;
				free(del);
				bst->size--;
			} else {
			    assert(0);
			}
		} else {
		    get_min_parent(del->right, &pa_min);
			assert(pa_min);
			min = pa_min->left;
			assert(min);
			pa_min->left = min->right;
			min->left = del->left;
			min->right = del->right;
			if (pa->left == del)
			    pa->left = min;
			else if (pa->right == del)
			    pa->right = min;
			else
			    assert(0);
			free(del);
			bst->size--;
		}
	}
	return DS_OK;
}

static ds_stat
insert(p_bst_node sub, bst_data_t data, bst_cmp cmp)
{
    p_bst_node   nd;

    if (!sub || !data)
	    return DS_ERROR;
	
	while (sub) {
	    if (cmp(data, sub->data) > 0) {
		    if (sub->right) {
			    sub = sub->right;
			} else {
			    if ((nd = create_node(data)) == NULL)
				    return DS_ERROR;
			    sub->right = nd;
				return DS_OK;
			}
		} else if (cmp(data, sub->data) < 0) {
		    if (sub->left) {
			    sub = sub->left;
			} else {
			    if ((nd = create_node(data)) == NULL)
				    return DS_ERROR;
			    sub->left = nd;
				return DS_OK;
			}
		} else {
		    return DS_ERROR;
		}
	}
	return DS_ERROR;
}

ds_stat
BSTree_Insert(p_bstree bst, bst_data_t data)
{
    if (!bst)
	    return DS_ERROR;
	if (bst->root == NULL) {
	    bst->root = create_node(data);
		bst->size++;
		return DS_OK;
	}

	if (insert(bst->root, data, bst->cmp) == DS_OK) {
	    bst->size++;
		return DS_OK;
	}
	return DS_ERROR;
}

size_t 
BSTree_Size(p_bstree bst)
{
    if (!bst)
	    return 0;
	return bst->size;
}
