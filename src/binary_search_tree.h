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

#ifndef GQRM_BINARY_SEARCH_TREE_H
#define GQRM_BINARY_SEARCH_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#include "header.h"


typedef struct _binary_search_tree    BSTree;
typedef BSTree*                       pt_BSTree;
typedef void*                         bst_data_t;
typedef int (*bst_cmp)(bst_data_t, bst_data_t);
typedef void (*bst_map_func)(bst_data_t, size_t);

#define p_bstree    pt_BSTree

extern p_bstree     BSTree_Create(bst_cmp);
extern ds_stat      BSTree_PreOrderMap(p_bstree, bst_map_func);
extern ds_stat      BSTree_InOrderMap(p_bstree, bst_map_func);
extern ds_stat      BSTree_PostOrderMap(p_bstree, bst_map_func);
extern ds_stat      BSTree_GetMax(p_bstree, bst_data_t*);
extern ds_stat      BSTree_GetMin(p_bstree, bst_data_t*);
extern ds_stat      BSTree_Insert(p_bstree, bst_data_t);
extern size_t       BSTree_Size(p_bstree);
extern ds_stat      BSTree_Delete(p_bstree, bst_data_t);

#undef p_bstree

#endif
