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
#include <stdio.h>
#include <assert.h>

#include "header.h"
#include "single_linked_list.h"

#define VERTEX_WEIGHT_INF    999

typedef struct VERTEX     Vertex;
typedef Vertex*           pt_Vertex;
typedef struct EDGE       Edge;
typedef Edge*             pt_Edge;
typedef struct ALGRAPH    ALGraph;
typedef ALGraph*          pt_ALGraph;

typedef void*             graph_data_t;
typedef double            edge_weight_t;
typedef int               vertex_weight_t;

typedef edge_weight_t (*is_neighbor)(graph_data_t, graph_data_t);

extern pt_Edge Edge_Create(pt_Vertex, const edge_weight_t);
extern ds_stat Edge_Assign(pt_Edge, pt_Edge);
extern ds_bool Edge_Same(pt_Edge, pt_Edge);
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


extern pt_Vertex Vertex_CreateDestination(gqrm_id_t, graph_data_t, vertex_weight_t);
extern pt_Vertex Vertex_CreateSource(gqrm_id_t, graph_data_t, vertex_weight_t);
extern pt_Vertex Vertex_CreateMediate(gqrm_id_t, graph_data_t, vertex_weight_t);
extern ds_stat   Vertex_Assign(pt_Vertex, pt_Vertex);
extern ds_bool   Vertex_Same(pt_Vertex, pt_Vertex);
extern ds_bool   Vertex_Equal(pt_Vertex, pt_Vertex);
extern ds_bool   Vertex_GreaterThan(pt_Vertex, pt_Vertex);
extern ds_bool   Vertex_NoLessThan(pt_Vertex, pt_Vertex);
extern ds_bool   Vertex_LessThan(pt_Vertex, pt_Vertex);
extern ds_bool   Vertex_NoGreaterThan(pt_Vertex, pt_Vertex);
extern ds_stat   Vertex_GetData(pt_Vertex, graph_data_t*);
extern ds_stat   Vertex_GetID(pt_Vertex, gqrm_id_t*);
extern ds_stat   Vertex_GetWeight(pt_Vertex, vertex_weight_t*);
extern ds_stat   Vertex_GetParent(pt_Vertex, gqrm_id_t*);
extern ds_stat   Vertex_GetEdges(pt_Vertex, p_sll*);
extern ds_stat   Vertex_SetData(pt_Vertex, graph_data_t);
extern ds_stat   Vertex_SetWeight(pt_Vertex, vertex_weight_t);
extern ds_stat   Vertex_SetParent(pt_Vertex, gqrm_id_t);
extern ds_bool   Vertex_IsDestination(pt_Vertex);
extern ds_bool   Vertex_IsSource(pt_Vertex);
extern ds_bool   Vertex_IsMediate(pt_Vertex);
extern ds_bool   Vertex_IsSelected(pt_Vertex);
extern ds_stat   Vertex_PushEdge(pt_Vertex, pt_Edge);
extern ds_stat   Vertex_PushNeighbor(pt_Vertex, pt_Vertex, edge_weight_t);
extern ds_bool   Vertex_IsNeighbor(pt_Vertex, pt_Vertex);
extern size_t    Vertex_Degree(pt_Vertex);
extern ds_stat   Vertex_PopEdge(pt_Vertex);
extern void      Vertex_ClearEdge(pt_Vertex);
extern void      Vertex_Free(pt_Vertex*);


extern pt_ALGraph    ALGraph_Create(void);
extern ds_stat       ALGraph_Init(pt_ALGraph, p_sll, is_neighbor);
extern ds_stat       ALGraph_Print(pt_ALGraph, FILE*);
#endif
