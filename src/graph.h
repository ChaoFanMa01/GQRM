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

#ifndef GQRM_GRAPH_H
#define GQRM_GRAPH_H

#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "single_linked_list.h"

typedef struct VERTEX     Vertex;
typedef Vertex*           pt_Vertex;
typedef struct EDGE       Edge;
typedef Edge*             pt_Edge;

typedef void*             graph_data_t;
typedef double            edge_weight_t;
typedef int               node_weight_t;

extern pt_Edge Edge_Create(pt_Vertex, const edge_weight_t);
extern ds_stat Edge_Assign(pt_Edge, pt_Edge);
extern ds_bool Edge_Equal(pt_Edge, pt_Edge);
extern ds_bool Edge_GreaterThan(pt_Edge, pt_Edge);
extern ds_bool Edge_NoLessThan(pt_Edge, pt_Edge);
extern ds_bool Edge_LessThan(pt_Edge, pt_Edge);
extern ds_bool Edge_NoGreaterThan(pt_Edge, pt_Edge);
extern void    Edge_Free(pt_Edge*);
extern ds_stat Edge_GetEnd(pt_Edge, pt_Vertex*);
extern ds_stat Edge_GetWeight(pt_Edge, edge_weight_t*);
extern ds_stat Edge_SetEnd(pt_Edge, pt_Vertex);
extern ds_stat Edge_SetWeight(pt_Edge, edge_weight_t);
#endif
