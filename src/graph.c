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
    vertex_weight_t     weight;
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

/* @struct
 * Structure defining a graph implemented based on 
 * adjacency list.
 */
struct ALGRAPH {
    p_sll    vertices;
};

static pt_Vertex create_vertex(gqrm_id_t, vertex_type, graph_data_t, vertex_weight_t, vertex_status, gqrm_id_t);
static int       edge_cmp(sll_data_t, sll_data_t);
static void      edge_clear_op(sll_data_t*);
static void      destroy_edges(pt_Vertex);
static void      vertex_clear_op(sll_data_t*);
static ds_stat   error_clear(pt_ALGraph);

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

    if (rhs->weight == lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Edge_Same(pt_Edge rhs, pt_Edge lhs)
{
    if (!rhs || !lhs)
        return DS_FALSE;

    if (rhs->end == lhs->end)
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
              graph_data_t d, vertex_weight_t w,
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
    if (SingleLinkedList_Init(&pv->edges) == DS_ERROR) {
        free(pv);
        return NULL;
    }
    return pv;
}

pt_Vertex
Vertex_CreateDestination(gqrm_id_t i, graph_data_t d,
                         vertex_weight_t w)
{
    return create_vertex(i, DST, d, w, S, -1);
}

pt_Vertex
Vertex_CreateSource(gqrm_id_t i, graph_data_t d,
                    vertex_weight_t w)
{
    return create_vertex(i, SRC, d, w, S, -1);
}

pt_Vertex
Vertex_CreateMediate(gqrm_id_t i, graph_data_t d,
                     vertex_weight_t w)
{
    return create_vertex(i, MDT, d, w, U, -1);
}

ds_stat
Vertex_Assign(pt_Vertex rhs, pt_Vertex lhs)
{
    p_sll cpy;

    if (!rhs || !lhs)
        return DS_ERROR;
 
    if ((cpy = SingleLinkedList_Copy(lhs->edges)) == NULL)
        return DS_ERROR;

    rhs->id     = lhs->id;
    rhs->type   = lhs->type;
    rhs->data   = lhs->data;
    rhs->weight = lhs->weight;
    rhs->status = lhs->status;
    rhs->parent = lhs->parent;

    return DS_OK;
}

/* @fn
 * Check whether two vertices are same based on 
 * their "id" fields.
 */
ds_bool
Vertex_Same(pt_Vertex rhs, pt_Vertex lhs)
{
    if (!rhs || !lhs)
        return DS_FALSE;

    if (rhs->id == lhs->id)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_Equal(pt_Vertex rhs, pt_Vertex lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight == lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_GreaterThan(pt_Vertex rhs, pt_Vertex lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight > lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_NoLessThan(pt_Vertex rhs, pt_Vertex lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight >= lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_LessThan(pt_Vertex rhs, pt_Vertex lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight < lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_NoGreaterThan(pt_Vertex rhs, pt_Vertex lhs)
{
    assert(rhs);
    assert(lhs);

    if (rhs->weight <= lhs->weight)
        return DS_TRUE;
    return DS_FALSE;
}

ds_stat
Vertex_GetData(pt_Vertex pv, graph_data_t* re)
{
    if (!pv || !re)
        return DS_ERROR;

    *re = pv->data;
    return DS_OK;
}

ds_stat
Vertex_GetID(pt_Vertex pv, gqrm_id_t* re)
{
    if (!pv || *re)
        return DS_ERROR;

    *re = pv->id;
    return DS_OK;
}

ds_stat
Vertex_GetWeight(pt_Vertex pv, vertex_weight_t* re)
{
    if (!pv || !re)
        return DS_ERROR;

    *re = pv->weight;
    return DS_OK;
}

ds_stat
Vertex_GetParent(pt_Vertex pv, gqrm_id_t* re)
{
    if (!pv || !re)
        return DS_ERROR;

    *re = pv->parent;
    return DS_OK;
}

ds_stat
Vertex_GetEdges(pt_Vertex pv, p_sll* re)
{
    if (!pv || !re)
        return DS_ERROR;

    *re = pv->edges;
    return DS_OK;
}

ds_stat
Vertex_SetData(pt_Vertex pv, graph_data_t d)
{
    if (!pv)
        return DS_ERROR;

    pv->data = d;
    return DS_OK;
}

ds_stat
Vertex_SetWeight(pt_Vertex pv, vertex_weight_t w)
{
    if (!pv)
        return DS_ERROR;

    pv->weight = w;
    return DS_OK;
}

ds_stat
Vertex_SetParent(pt_Vertex pv, gqrm_id_t p)
{
    if (!pv)
        return DS_ERROR;

    pv->parent = p;
    return DS_OK;
}

ds_bool
Vertex_IsDestination(pt_Vertex pv)
{
    if (!pv)
        return DS_FALSE;

    if (pv->type == DST)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_IsSource(pt_Vertex pv)
{
    if (!pv)
        return DS_FALSE;

    if (pv->type == SRC)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_IsMediate(pt_Vertex pv)
{
    if (!pv)
        return DS_FALSE;

    if (pv->type == MDT)
        return DS_TRUE;
    return DS_FALSE;
}

ds_bool
Vertex_IsSelected(pt_Vertex pv)
{
    if (!pv)
        return DS_FALSE;

    if (pv->status == S)
        return DS_TRUE;
    return DS_FALSE;
}

ds_stat
Vertex_PushNeighbor(pt_Vertex pv, pt_Vertex n, edge_weight_t w)
{
    pt_Edge pe = Edge_Create(n, w);

    if (!pe)
        return DS_ERROR;
    
    return Vertex_PushEdge(pv, pe);
}

ds_stat
Vertex_PushEdge(pt_Vertex pv, pt_Edge pe)
{
    if (!pv || !pe)
        return DS_ERROR;

    if (!SingleLinkedList_Contain(pv->edges, pe, edge_cmp)) {
        SingleLinkedList_InsertHead(pv->edges, pe);
        return DS_OK;
    }
    return DS_ERROR;
}

ds_stat
Vertex_PopEdge(pt_Vertex pv)
{
    pt_Edge pe;
    if (!pv)
        return DS_ERROR;
    if (SingleLinkedList_DeleteHead(pv->edges, (sll_data_t*)&pe) == DS_OK) {
        Edge_Free(&pe);
        return DS_OK;
    }
    return DS_ERROR;
}

void
Vertex_ClearEdge(pt_Vertex pv)
{
    SingleLinkedList_Clear(pv->edges, edge_clear_op);
}

static void
destroy_edges(pt_Vertex pv)
{
    SingleLinkedList_Destroy(&pv->edges, edge_clear_op);
}

void
Vertex_Free(pt_Vertex* pv)
{
    if (!pv)
        return;
    destroy_edges(*pv);
    free(*pv);
    *pv = NULL;
}

size_t
Vertex_Degree(pt_Vertex pv)
{
    if (!pv)
        return 0;
    return SingleLinkedList_Size(pv->edges);
}

ds_bool
Vertex_IsNeighbor(pt_Vertex pv, pt_Vertex n)
{
    pt_Edge pe;
    if (!pv || !n)
        return DS_FALSE;

    if ((pe = Edge_Create(n, 0.0)) == NULL)
        return DS_FALSE;
    if (SingleLinkedList_Contain(pv->edges, pe, edge_cmp)) {
        Edge_Free(&pe);
        return DS_TRUE;
    } else {
        Edge_Free(&pe);
        return DS_FALSE;
    }
        
}

static int
edge_cmp(sll_data_t rhs, sll_data_t lhs)
{
    if (Edge_Same((pt_Edge)rhs, (pt_Edge)lhs) == DS_TRUE)
        return 0;
    return 1;
}

static void
edge_clear_op(sll_data_t* e)
{
    pt_Edge* pe;
    if (!e)
        return;

    pe = (pt_Edge*)e;
    Edge_Free(pe);
}

pt_ALGraph
ALGraph_Create(void)
{
    pt_ALGraph   pg = malloc(sizeof(ALGraph));

    if (!pg)
        return NULL;

    if (SingleLinkedList_Init(&pg->vertices) == DS_ERROR) {
        free(pg);
        return NULL;
    }
    return pg;
}

/* @fn 
 * Initialize a adjacency list based graph according to
 * a linked list "init_list", which stores all the data
 * to be assigned to vertices in this graph.
 * 
 * @param pg An empty graph to be initialized.
 * @param init_list The list storing all data.
 * @param func A callback function to determine whether
 *        two vertices are neighbors in this graph.
 */
ds_stat
ALGraph_Init(pt_ALGraph pg, p_sll init_list, is_neighbor func)
{
    size_t           i, j, size;
    graph_data_t     data;
    pt_Vertex        pv, pi, pj;
    edge_weight_t    w;

    if (!pg || !init_list)
        return DS_ERROR;

    size = SingleLinkedList_Size(init_list);
    for (i = 0; i < size; i++) {
        if (SingleLinkedList_GetData(init_list, i, &data) == DS_ERROR)
            return error_clear(pg);
        if ((pv = Vertex_CreateMediate(i, data, VERTEX_WEIGHT_INF)) == NULL)
            return error_clear(pg);
        if (SingleLinkedList_InsertTail(pg->vertices, pv) == DS_ERROR)
            return error_clear(pg);
    }

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            if (i != j) {
                if (SingleLinkedList_GetData(pg->vertices, i, (sll_data_t*)&pi) == DS_ERROR)
                    return error_clear(pg);
                if (SingleLinkedList_GetData(pg->vertices, j, (sll_data_t*)&pj) == DS_ERROR)
                    return error_clear(pg);
                if ((w = func(pi->data, pj->data)) > 0.0)
                    if (Vertex_PushNeighbor(pi, pj, w) == DS_ERROR)
                        return error_clear(pg);
            }
    return DS_OK;
}

static ds_stat
error_clear(pt_ALGraph pg)
{
    if (!pg)
        return DS_ERROR;
    SingleLinkedList_Clear(pg->vertices, vertex_clear_op);
    return DS_ERROR;
}

static void
vertex_clear_op(sll_data_t* v)
{
    pt_Vertex* pv;
    if (!v)
        return;

    pv = (pt_Vertex*)v;
    Vertex_Free(pv);
}

ds_stat
ALGraph_Print(pt_ALGraph pg, FILE* fp)
{
    size_t     v_size, e_size, i, j;
    pt_Vertex  pv;
    pt_Edge    pe;
    if (!pg || !fp)
        return DS_ERROR;

    v_size = SingleLinkedList_Size(pg->vertices);
    for (i = 0; i < v_size; i++) {
        if (SingleLinkedList_GetData(pg->vertices, i, (sll_data_t*)&pv) == DS_ERROR)
            return DS_ERROR;
        fprintf(fp, "id: %4ld, weight: %3d, parent: %4ld, edges: ", pv->id, pv->weight, pv->parent);
        e_size = SingleLinkedList_Size(pv->edges);
        for (j = 0; j < e_size; j++) {
            if (SingleLinkedList_GetData(pv->edges, j, (sll_data_t*)&pe) == DS_ERROR)
                return DS_ERROR;
            fprintf(fp, "->(id: %4ld, weight: %2.4lf) ", pe->end->id, pe->weight);
        }
        fprintf(fp, "\n");
    }
}
