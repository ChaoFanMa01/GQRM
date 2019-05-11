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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/header.h"
#include "../src/node.h"
#include "../src/single_linked_list.h"
#include "../src/graph.h"

edge_weight_t checker(graph_data_t, graph_data_t);

int main(int argc, char* argv[])
{
    pt_Node       nd;
	size_t        size, i;
	pt_ALGraph    pg;
	p_sll         nodes;

	srand((unsigned)time(0));

	if (argc != 2)
	    exit(-1);
	size = atoi(argv[1]);

    if (SingleLinkedList_Init(&nodes) == DS_ERROR)
	    exit(-1);
	for (i = 0; i < size; i++) {
	    if ((nd = Node_CreateRandomCDL(i, 10.0, 10)) == NULL)
		    exit(-1);
	    if (SingleLinkedList_InsertTail(nodes, nd) == DS_ERROR)
		    exit(-1);
	    Node_2DPrint(nd, stdout); printf("\n");
	}
	if ((pg = ALGraph_Create()) == NULL)
	    exit(-1);
    if (ALGraph_Init(pg, nodes, checker) == DS_ERROR)
	    exit(-1);
    ALGraph_Print(pg, stdout);
	return 0;
}

edge_weight_t
checker(graph_data_t d1, graph_data_t d2)
{
    double   prr;
	pt_Node  nd1 = (pt_Node)d1;
	pt_Node  nd2 = (pt_Node)d2;

	if (Node_IsNeighbor(nd1, nd2, &prr) == DS_TRUE)
	    return prr;
    return -1.0;
}
