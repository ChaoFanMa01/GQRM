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

#ifndef GQRM_RB_TREE_H
#define GQRM_RB_TREE_H

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "header.h"

typedef struct _rb_tree      RBTree;
typedef RBTree*              pt_RBTree;
typedef void*                rbt_data_t;
typedef int (*rbt_cmp)(rbt_data_t, rbt_data_t);
typedef void (*rbt_map_func)(rbt_data_t, size_t);

extern pt_RBTree RBTree_Create(rbt_cmp);
extern ds_stat   RBTree_PreOrderMap(pt_RBTree, rbt_map_func);
extern ds_stat   RBTree_InOrderMap(pt_RBTree, rbt_map_func);
extern ds_stat   RBTree_PostOrderMap(pt_RBTree, rbt_map_func);
extern ds_stat   RBTree_Insert(pt_RBTree, rbt_data_t);
extern size_t    RBTree_Size(pt_RBTree);

#endif
