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

#ifndef GQRM_SET_H
#define GQRM_SET_H

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include "header.h"
#include "avl_tree.h"

typedef struct _set       Set;
typedef Set*              pt_Set;
typedef void*             set_data_t;

typedef int (*set_cmp)(set_data_t, set_data_t);
typedef void (*set_map)(set_data_t, size_t);

extern pt_Set      Set_Create(set_cmp, gqrm_id_t);
extern ds_stat     Set_Init(pt_Set, set_data_t [], size_t);
extern ds_stat     Set_Insert(pt_Set, set_data_t);
extern ds_stat     Set_Delete(pt_Set, set_data_t);
extern size_t      Set_Size(pt_Set);
extern pt_Set      Set_Minus(pt_Set, pt_Set);
extern ds_stat     Set_InSetMinus(pt_Set, pt_Set);
extern pt_Set      Set_Union(pt_Set, pt_Set);
extern ds_stat     Set_InSetUnion(pt_Set, pt_Set);
#endif
