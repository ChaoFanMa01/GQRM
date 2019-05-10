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

#include "graph.h"

/* @enum
 * Enumerate type identifying the type of a vertex:
 * DST - a destination vertex
 * SRC - a source vertex
 * MDT - a mediate vertex
 */
typedef enum {
    DST, SRC, MDT
} vertex_type;

/* @enum
 * Enumerate type identifying the status of a vertex:
 * S - selected to build a subgraph
 * U - unselected to build a subgraph
 */
typedef enum {
    S, U
} vertex_status;

/* @struct
 * Structure defining a vertex in graph.
 * id     - the series number of this vertex
 * type   - the type of this vertex
 * data   - user data storing in this vertex
 * weight - vertex weight
 * status - 
 * parent - while building a tree, this field point to 
 *          the parent of this vertex
 * edges  - edges adjacent to this vertex
 */
struct VERTEX {
    gqrm_id_t         id;
    vertex_type       type;
    graph_data_t      data;
    node_weight_t     weight;
    vertex_status     status;
    gqrm_id_t         parent;
    p_sll             edges;
};

/* @struct
 * Structure defining an edge in graph.
 * end     - an end of this edge (the other is the vertex
 *           where this edge is stored)
 * weight  - edge weight
 */
struct EDGE {
    pt_Vertex         end;
    edge_weight_t     weight;
};

pt_Edge
Edge_Create(pt_Vertex v, const edge_weight_t w)
{
    pt_Edge pe;

    if (!v)
        return NULL;

    if ((pe = malloc(sizeof(Edge))) == NULL)
        return NULL;
    pe->end    = v;
    pe->weight = w;
    return pe;
}

/* @fn
 * rhs = lhs
 */
ds_stat
Edge_Assign(pt_Edge rhs, pt_Edge lhs)
{
    if (!rhs || !lhs)
        return DS_ERROR;

    assert(rhs);
    assert(lhs);

    rhs->end    = lhs->end;
    rhs->weight = lhs->weight;
    return DS_OK;
}

/* @fn
 * Check whether two edges are equal.
 */
ds_bool
Edge_Equal(pt_Edge rhs, pt_Edge lhs)
{
    if (!rhs || !lhs)
        return DS_FALSE;

    if (
        rhs->end == lhs->end ||
        rhs->weight == lhs->weight
       )
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn
 * check whether the weight of "rhs" is greater than
 * the weight of "lhs".
 */
ds_bool
Edge_GreaterThan(pt_Edge rhs, pt_Edge lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight > lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn
 * Check whether the weight of "rhs" is greater than or
 * equal to the weight of "lhs".
 */
ds_bool
Edge_NoLessThan(pt_Edge rhs, pt_Edge lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight >= lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Edge_LessThan(pt_Edge rhs, pt_Edge lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight < lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Edge_NoGreaterThan(pt_Edge rhs, pt_Edge lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight <= lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

void
Edge_Free(pt_Edge* pe)
{
    if (!*pe)
        return;
    free(*pe);
    *pe = NULL;
}

ds_stat
Edge_GetEnd(pt_Edge pe, pt_Vertex* re)
{
    if (!pe)
        return DS_ERROR;
    *re = pe->end;
    return DS_OK;
}

ds_stat
Edge_GetWeight(pt_Edge pe, edge_weight_t* re)
{
    if (!pe) 
        return DS_ERROR;
    *re = pe->weight;
    return DS_OK;
}

ds_stat
Edge_SetEnd(pt_Edge pe, pt_Vertex v)
{
    if (!pe || !v)
        return DS_ERROR;
    pe->end = v;
    return DS_OK;
}

ds_stat
Edge_SetWeight(pt_Edge pe, edge_weight_t w)
{
    if (!pe)
        return DS_ERROR;
    pe->weight = w;
    return DS_OK;
}

static pt_Vertex
create_vertex(gqrm_id_t i, vertex_type t, 
              graph_data_t d, node_weight_t w,
              vertex_status s, gqrm_id_t p)
{
    pt_Vertex   pv = malloc(sizeof(Vertex));

    if (!pv)
        return NULL;

    pv->id      = i;
    pv->type    = t;
    pv->data    = d;
    pv->weight  = w;
    pv->status  = s;
    pv->parent  = p;
    SingleLinkedList_Init(&pv->edges);
}
