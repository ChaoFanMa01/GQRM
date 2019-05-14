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

#include "rnp_misc.h"

static ds_bool error_clear(pt_ALGraph*);
static ds_bool is_dst(gqrm_id_t[], size_t, gqrm_id_t);

/* @fn 
 * Check whether the given graph has a feasible 
 * solution to a hop constrained relay node placement
 * problem. Two aspects are checked in this function,
 * i.e., 
 * 1) all paths between the source vertex and 
 * each destination vertex fulfill hop constraints;
 * 2) no destination vertex is isolated from the 
 * source vertex.
 */
ds_bool
check_feasibility(pt_ALGraph pg, gqrm_id_t src, 
                  gqrm_id_t dsts[], size_t n)
{
    pt_ALGraph         spt = NULL; 
	pt_ALGraph         cpy = NULL;
	size_t             size, i;
	pt_Vertex          pv = NULL;
	vertex_weight_t    hop;
	pt_Node            pn = NULL;
	gqrm_id_t          id, parent;
	gqrm_hop_t         hop_constraint;

	if (!pg)
	    return DS_FALSE;
	if ((spt = ALGraph_ShortestPathTree(pg, src, dsts, n)) == NULL)
	    return DS_FALSE;

	size = ALGraph_Size(pg);
	for (i = 0; i < size; i++) {
	    if (ALGraph_GetVertex(spt, i, &pv) == DS_ERROR)
		    return error_clear(&spt);
		if (Vertex_GetID(pv, &id) == DS_ERROR)
		    return error_clear(&spt);
		/* we should only check the destination vertex */
		if (is_dst(dsts, n, id) == DS_TRUE) {
		    /* get its least hop count to the source vertex */
		    if (Vertex_GetWeight(pv, &hop) == DS_ERROR)
		        return error_clear(&spt);
    		if (Vertex_GetData(pv, (graph_data_t)&pn) == DS_ERROR)
	    	    return error_clear(&spt);
			/* get the hop constraint imposed on this vertex */
		    if (Node_GetHop(pn, &hop_constraint) == DS_ERROR)
		        return error_clear(&spt);
			/* check whether hop constraint is met */
		    if (hop > hop_constraint) {
		        return error_clear(&spt);
			}
			/* 
			 * check whether this destination is isolated from 
			 * the srouce vertex.
			 */
			while (id != -1) {
		        if (Vertex_GetParent(pv, &parent) == DS_ERROR)
		            return error_clear(&spt);
				if (parent == src) {
				    break;
				} else if (parent == -1) {
				    return error_clear(&spt);
				} else {
				    id = parent;
					if (ALGraph_GetVertexByID(spt, id, &pv) == DS_ERROR)
		                return error_clear(&spt);
				}
			}
		}
	}
	ALGraph_Free(&spt);
	return DS_TRUE;
}

edge_weight_t 
check_neighbor(graph_data_t d1, graph_data_t d2)
{ 
    double   prr;
	pt_Node  nd1 = (pt_Node)d1;
	pt_Node  nd2 = (pt_Node)d2;

	if (Node_IsNeighbor(nd1, nd2, &prr) == DS_TRUE)
	    return prr;
    return -1.0;
}

static ds_bool
is_dst(gqrm_id_t dsts[], size_t n, gqrm_id_t id)
{
    size_t  i;

	for (i = 0; i < n; i++)
	    if (dsts[i] == id)
		    return DS_TRUE;
	return DS_FALSE;
}

static ds_bool
error_clear(pt_ALGraph* pg) 
{
    ALGraph_Free(pg);
	return DS_FALSE;
}

/* 
 * Check whether a vertex's data field store a 
 * sensor node.
 */
ds_bool
is_VertexSN(pt_Vertex pv)
{
    pt_Node  pn;

	assert(pv);

	if (Vertex_GetData(pv, (graph_data_t*)&pn) == DS_ERROR)
	    assert(0);
	return Node_IsSN(pn);
}

/* 
 * Check whether a vertex's data field store a 
 * candidate deployment location.
 */
ds_bool
is_VertexCDL(pt_Vertex pv)
{
    pt_Node  pn;

	assert(pv);

	if (Vertex_GetData(pv, (graph_data_t*)&pn) == DS_ERROR)
	    assert(0);
	return Node_IsCDL(pn);
}

/* 
 * Check whether a vertex's data field store a 
 * gateway.
 */
ds_bool
is_VertexGW(pt_Vertex pv)
{
    pt_Node  pn;

	assert(pv);

	if (Vertex_GetData(pv, (graph_data_t*)&pn) == DS_ERROR)
	    assert(0);
	return Node_IsGW(pn);
}
