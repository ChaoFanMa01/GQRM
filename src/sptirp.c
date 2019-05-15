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

#include "sptirp.h"

static pt_ALGraph error_clear(pt_ALGraph*, pt_ALGraph*);
static ds_bool is_in(gqrm_id_t [], size_t, gqrm_id_t);

pt_ALGraph
SPTiRP(p_sll nodes)
{
    pt_ALGraph      spt, pg;
	pt_Node         pn;
	pt_Vertex       pv, pv_pa;
	gqrm_id_t       src = -1, id, parent;
	gqrm_id_t       dsts[200], cdls[400];
	size_t          n, i, size, n_cdls;

    printf("get sns and gw\n");
    /* get all sensor nodes and gateway */
	size = SingleLinkedList_Size(nodes);
	for (i = 0, n = 0; i < size; i++) {
	    if (SingleLinkedList_GetData(nodes, i, (sll_data_t*)&pn) == DS_ERROR)
		    return NULL;
		if (Node_IsSN(pn) == DS_TRUE) {
		    if (Node_GetID(pn, &dsts[n++]) == DS_ERROR)
			    return NULL;
		} else if (Node_IsGW(pn) == DS_TRUE) {
		    assert(src == -1);
		    if (Node_GetID(pn, &src) == DS_ERROR)
			    return NULL;
		}
	}

    printf("check feasibility\n");
    /* initialize pg and spt, and check feasibility */
	if ((pg = ALGraph_Create()) == NULL)
	    return NULL;
	if (ALGraph_Init(pg, nodes, check_neighbor) == DS_ERROR)
	    return error_clear(&pg, NULL);
	if (check_feasibility(pg, src, dsts, n) == DS_FALSE)
	    return error_clear(&pg, NULL);
	if ((spt = ALGraph_ShortestPathTree(pg, src, dsts, n)) == NULL)
	    return error_clear(&pg, &spt);

    
    printf("get cdls\n");
    /* get all CDLs on the original shortest path tree */
	for (i = 0, n_cdls; i < n; i++) {
	    if (ALGraph_GetVertexByID(spt, dsts[i], &pv) == DS_ERROR)
		    return error_clear(&pg, &spt);
		if (Vertex_GetParent(pv, &parent) == DS_ERROR)
		    return error_clear(&pg, &spt);
		while (parent != src) {
			assert(parent != -1);
		    if (ALGraph_GetVertexByID(spt, parent, &pv) == DS_ERROR)
		        return error_clear(&pg, &spt);
			if (is_VertexCDL(pv) == DS_TRUE)
			    if (is_in(cdls, n_cdls, parent) == DS_FALSE)
    			    cdls[n_cdls++] = parent;
			if (Vertex_GetParent(pv, &parent) == DS_ERROR)
		        return error_clear(&pg, &spt);
		}
	}

    printf("unselecting\n");
    /* unselect all CDLs not on the original shortest path tree */
    for (i = 0; i < size; i++) {
	    if (SingleLinkedList_GetData(nodes, i, (sll_data_t*)&pn) == DS_ERROR)
		    return error_clear(&pg, &spt);
		if (Node_GetID(pn, &id) == DS_ERROR)
		    return error_clear(&pg, &spt);
		if (Node_IsCDL(pn) == DS_TRUE && is_in(dsts, n_cdls, id) == DS_FALSE)
		    Node_SetUnselected(pn);
	}

    ALGraph_Free(&spt);

    printf("pruning\n");
	/* prune redundant CDLs */
	for (i = 0; i < n_cdls; i++) {
	    if (ALGraph_GetVertexByID(pg, cdls[i], &pv) == DS_ERROR)
		    return error_clear(&pg, NULL);
		if (Vertex_GetData(pv, (graph_data_t*)&pn) == DS_ERROR)
		    return error_clear(&pg, NULL);
		if (Node_GetID(pn, &id) == DS_ERROR)
		    return error_clear(&pg, NULL);
		assert(cdls[i] == id);
		Node_SetUnselected(pn);
		ALGraph_Free(&pg);
		if ((pg = ALGraph_Create()) == NULL)
		    return NULL;
		if (ALGraph_Init(pg, nodes, check_neighbor) == DS_ERROR)
		    return error_clear(&pg, NULL);
		if (check_feasibility(pg, src, dsts, n) == DS_FALSE) {
		    ALGraph_Free(&pg);
			Node_SetSelected(pn);
			if ((pg = ALGraph_Create()) == NULL)
			    return NULL;
		    if (ALGraph_Init(pg, nodes, check_neighbor) == DS_ERROR)
    		    return error_clear(&pg, NULL);
		} else {
		    printf("delete %ld\n", cdls[i]);
		}
	}
	return pg;
}

static ds_bool is_in(gqrm_id_t ids[], size_t n, gqrm_id_t id)
{
    size_t i;
	for (i = 0; i < n; i++)
	    if (ids[i] == id)
		    return DS_TRUE;
	return DS_FALSE;
}

static pt_ALGraph error_clear(pt_ALGraph* pg1, pt_ALGraph* pg2)
{
    if (pg1)
	    ALGraph_Free(pg1);
	if (pg2)
	    ALGraph_Free(pg2);
	return NULL;
}
