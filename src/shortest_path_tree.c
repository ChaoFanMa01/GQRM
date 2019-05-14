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

#include "shortest_path_tree.h"

static ds_bool input_feasibility(pt_ALGraph, gqrm_id_t, gqrm_id_t [], size_t);
static pt_ALGraph error_clear(pt_ALGraph*, p_sll*, p_sll*, p_sll*);
static int vertex_cmp(sll_data_t, sll_data_t);
static ds_bool is_dst_or_src(gqrm_id_t, gqrm_id_t [], size_t, gqrm_id_t);
static ds_bool is_dst(gqrm_id_t [], size_t, gqrm_id_t);
static ds_bool has_non_dst_leaves(pt_ALGraph, gqrm_id_t[], size_t);

/* @fn
 * Create a shortest path tree based on 
 * Dijkstra's method.
 *
 *              /\
 *             /  \
 *            /gray\     N
 *  ---------/empty?\------------------------->pruning process
 *  |        \      /
 *  |         \    /
 *  |          \  /
 *  |           \/
 *  |            |
 *  |           \|/
 *  |    ----------------------------------
 *  |    | u = min weigted vertex in gray |
 *  |    ----------------------------------
 *  |                  |
 *  |                 \|/
 *  |    ----------------------------------
 *  |    | add u to black                 |
 *  |    ----------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | add edges between u and its parent |
 *  |    --------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | delete u from gray                 |
 *  |    --------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | A = all neighbors of u in gray     |
 *  |    -------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | foreach v in A:                    |
 *  |    |     if v->weight > u->weight + 1:  |
 *  |    |         v->weight = u->weight + 1; |
 *  |    |         v->parent = u;             |
 *  |    --------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | B = all neighbors of u in white    |
 *  |    -------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | add vertices in B to gray          |
 *  |    -------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | delete vertices in B from white    |
 *  |    -------------------------------------
 *  |                  |
 *  |                 \|/
 *  |    --------------------------------------
 *  |    | for each v in B:                   |
 *  |    |     v->weight = u->weight + 1;     |
 *  |    |     v->parent = u;                 |
 *  |    --------------------------------------
 *  |                  |
 *  |                 \|/
 *  |                  |
 *  --------------------
 */
pt_ALGraph
ALGraph_ShortestPathTree(pt_ALGraph pg, gqrm_id_t src,
                         gqrm_id_t dsts[], size_t n)
{
    pt_ALGraph       spt = NULL;
	gqrm_id_t        id, parent;
	size_t           size, i, index;
	edge_weight_t    edge_weight;
	vertex_weight_t  vertex_weight1, vertex_weight2;
	pt_Vertex        pv = NULL, pv_tmp = NULL, pv_new = NULL, min = NULL, pv_parent = NULL;
	pt_Edge          pe = NULL;
	p_sll            gray = NULL;
	p_sll            white = NULL;
	p_sll            black = NULL;


    /*
	 * gray  = \varnothing;
	 * white = \varnothing;
	 * black = \varnothing;
	 */
	if (input_feasibility(pg, src, dsts, n) == DS_FALSE)
	    return NULL;
	if (SingleLinkedList_Init(&gray) == DS_ERROR)
	    return NULL;
	if (SingleLinkedList_Init(&white) == DS_ERROR) {
	    SingleLinkedList_Destroy(&gray, NULL);
	    return NULL;
	}
	if (SingleLinkedList_Init(&black) == DS_ERROR) {
	    SingleLinkedList_Destroy(&gray, NULL);
	    SingleLinkedList_Destroy(&white, NULL);
	    return NULL;
	}

    /* create a graph without any edge */
	spt = ALGraph_Create();
	size = ALGraph_Size(pg);
	for (i = 0; i < size; i++) {
	    if (ALGraph_GetVertex(pg, i, &pv) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		if ((pv_new = Vertex_ShallowCopy(pv)) == NULL)
		    return error_clear(&spt, &gray, &white, &black);
		if (ALGraph_PushVertex(spt, pv_new) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
	}

    /*
	 * copy src; src->weight = 0;
	 * add src to gray;
	 */
    if (ALGraph_GetVertex(spt, src, &pv) == DS_ERROR)
	    return error_clear(&spt, &gray, &white, &black);
	Vertex_SetWeight(pv, 0);
	if (SingleLinkedList_InsertHead(gray, pv) == DS_ERROR)
	    return error_clear(&spt, &gray, &white, &black);
	/*
	 * white = all vertices except for src.
	 */
	for (i = 0; i < size; i++) {
	    if (ALGraph_GetVertex(spt, i, &pv_tmp) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		/* if src != pv, add pv to white */
		if (Vertex_Same(pv, pv_tmp) != DS_TRUE)
			if (SingleLinkedList_InsertHead(white, pv_tmp) == DS_ERROR)
			    return error_clear(&spt, &gray, &white, &black);
	}

    /* loop until gray is empty */
	while (SingleLinkedList_Empty(gray) != DS_TRUE) {
	    /* get index of the vertex with minimum weight */
	    if (SingleLinkedList_Min(gray, vertex_cmp, &index) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		/* get the minimal weighted vertex */
		if (SingleLinkedList_GetData(gray, index, (sll_data_t*)&min) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);

		/* add min to black */
		if (SingleLinkedList_InsertHead(black, min) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		/* delete min from gray */
		if (SingleLinkedList_Delete(gray, index, NULL) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);

        /*
		 * add edges between min and its parent
		 */
		if (Vertex_GetParent(min, &parent) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		if (parent >= 0) {
		    /* get the parent vertex of min */
		    if (ALGraph_GetVertexByID(spt, parent, &pv_parent) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
		    if (ALGraph_GetVertexByID(pg, parent, &pv_tmp) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			if (Vertex_GetID(min, &id) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			if (Vertex_GetEdgeWeight(pv_tmp, id, &edge_weight) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			/* add an edge (to min's parent) between min and its parent */
			if (Vertex_PushNeighbor(pv_parent, min, edge_weight) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			/* add an edge (to min) between min and its parent */
//			if (Vertex_PushNeighbor(min, pv_parent, edge_weight) == DS_ERROR)
//		        return error_clear(&spt, &gray, &white, &black);
		}

        /*
		 * A = all neighbors of min in gray;
		 * foreach v in A:
		 *     if (v->weight > min->weight + 1):
		 *         v->weight = min->weight + 1;
		 *         v->parent = min;
		 */
		/* get min's id */
		if (Vertex_GetID(min, &id) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		/* get the min in pg so as to find all min's neighbors */
	    if (ALGraph_GetVertexByID(pg, id, &pv_tmp) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		/* get min's weight */
		if (Vertex_GetWeight(min, &vertex_weight1) == DS_ERROR)
		    return error_clear(&spt, &gray, &white, &black);
		size = SingleLinkedList_Size(gray);
		/* for each vertex in gray */
		for (i = 0; i < size; i++) {
		    /* get the ith vertex in gray */
		    if (SingleLinkedList_GetData(gray, i, (sll_data_t*)&pv) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			/* check whether "pv" is a neighbor of min */
			if (Vertex_IsNeighbor(pv_tmp, pv) == DS_TRUE) {
			/* if so */
			    /* get the weight of "pv" */
			    if (Vertex_GetWeight(pv, &vertex_weight2) == DS_ERROR)
		            return error_clear(&spt, &gray, &white, &black);
				/* if pv->weight > min->weight + 1*/
				if (vertex_weight2 > vertex_weight1 + 1) {
				    /* pv->weight = min->weight + 1*/
				    Vertex_SetWeight(pv, vertex_weight1 + 1);
					/* pv->parent = min */
					Vertex_SetParent(pv, id);
				}
			}
		}

//        printf("func: %s, line: %d\n", __func__, __LINE__);
        /*
		 * B = all neighbors of min in white;
		 * delete B from white;
		 * add B to gray;
		 * foreach v in B:
		 *     v->weight = min->weight + 1;
		 *     v->parent = min;
		 */
		for (i = 0; i < SingleLinkedList_Size(white);) {
		    /* get the ith vertex in white */
		    if (SingleLinkedList_GetData(white, i, (sll_data_t*)&pv) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			/* check whether "pv" is a neighbor of min */
			if (Vertex_IsNeighbor(pv_tmp, pv) == DS_TRUE) {
			/* if so */
			    /* pv->weight = min->weight + 1 */
			    Vertex_SetWeight(pv, vertex_weight1 + 1);
				/* pv->parent = min */
				Vertex_SetParent(pv, id);
				/* delete pv from white */
				if (SingleLinkedList_Delete(white, i, NULL) == DS_ERROR)
		            return error_clear(&spt, &gray, &white, &black);
				/* add pv to gray */
				if (SingleLinkedList_InsertHead(gray, pv) == DS_ERROR)
		            return error_clear(&spt, &gray, &white, &black);
			} else {
			    i++;
			}
		}
	}
    size = ALGraph_Size(spt);
	while (has_non_dst_leaves(spt, dsts, n) == DS_TRUE) {
	    for (i = 0; i < size; i++) {
		    if (ALGraph_GetVertex(spt, i, &pv) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			if (Vertex_GetID(pv, &id) == DS_ERROR)
		        return error_clear(&spt, &gray, &white, &black);
			if (
			    Vertex_Degree(pv) <= 0 && 
				is_dst(dsts, n, id) == DS_FALSE
			   ) {
			    if (Vertex_GetParent(pv, &parent) == DS_ERROR)
		            return error_clear(&spt, &gray, &white, &black);
				if (parent == -1)
				    continue;
				if (ALGraph_GetVertexByID(spt, parent, &pv_parent) == DS_ERROR)
		            return error_clear(&spt, &gray, &white, &black);
				if (Vertex_DeleteEdge(pv_parent, id) == DS_ERROR)
		            return error_clear(&spt, &gray, &white, &black);
				Vertex_SetParent(pv, -1);
			}
		}
	}

	SingleLinkedList_Destroy(&gray, NULL);
	SingleLinkedList_Destroy(&white, NULL);
	SingleLinkedList_Destroy(&black, NULL);
//    printf("func: %s, line: %d\n", __func__, __LINE__);

	return spt;
}

static ds_bool
has_non_dst_leaves(pt_ALGraph pg, gqrm_id_t dsts[], size_t n)
{
    pt_Vertex   pv;
	gqrm_id_t   id, parent;

    assert(pg);

    size_t size, i;
	size = ALGraph_Size(pg);
	for (i = 0; i < size; i++) {
	    if (ALGraph_GetVertex(pg, i, &pv) == DS_ERROR)
		    assert(0);
		if (Vertex_GetID(pv, &id) == DS_ERROR)
		    assert(0);
		if (Vertex_GetParent(pv, &parent) == DS_ERROR)
		    assert(0);
		if (
		    Vertex_Degree(pv) <= 0 && 
			is_dst(dsts, n, id) == DS_FALSE &&
			parent != -1
		   ) {
		    return DS_TRUE;
		}
	}
	return DS_FALSE;
}

static ds_bool
is_dst(gqrm_id_t dsts[], size_t n, gqrm_id_t d)
{
    size_t i;
	for (i = 0; i < n; i++)
	    if (dsts[i] == d)
		    return DS_TRUE;
	return DS_FALSE;
}

static ds_bool
is_dst_or_src(gqrm_id_t src, gqrm_id_t dsts[], size_t n, gqrm_id_t d)
{
    size_t   i;
	if (src == d)
	    return DS_TRUE;
    for (i = 0; i < n; i++)
	    if (dsts[i] == d)
		    return DS_TRUE;
	return DS_FALSE;
}

static int 
vertex_cmp(sll_data_t v1, sll_data_t v2)
{
	vertex_weight_t   w1, w2;

    assert(v1);
	assert(v2);
	
	Vertex_GetWeight((pt_Vertex)v1, &w1);
	Vertex_GetWeight((pt_Vertex)v2, &w2);
	return w1 - w2;
}

static pt_ALGraph
error_clear(pt_ALGraph* pg, p_sll* g, p_sll* w, p_sll* b)
{
    ALGraph_Free(pg);
    SingleLinkedList_Destroy(g, NULL);
    SingleLinkedList_Destroy(w, NULL);
    SingleLinkedList_Destroy(b, NULL);
	return NULL;
}

static ds_bool
input_feasibility(pt_ALGraph pg, gqrm_id_t src, 
                  gqrm_id_t dsts[], size_t n)
{
    size_t    i;

	if (!pg || ALGraph_Size(pg) <= 0)
	    return DS_FALSE;
	if (ALGraph_ContainVertexID(pg, src) == DS_FALSE)
	    return DS_FALSE;
	for (i = 0; i < n; i++)
	    if (ALGraph_ContainVertexID(pg, dsts[i]) == DS_FALSE)
		    return DS_FALSE;
	return DS_TRUE;
}
