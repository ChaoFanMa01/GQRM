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
#include "../src/shortest_path_tree.h"
#include "../src/mysql_api.h"

edge_weight_t checker(graph_data_t, graph_data_t);

int main(int argc, char* argv[])
{
    pt_Node       nd;
	size_t        size, i;
	pt_ALGraph    pg, cpy, spt;
	p_sll         nodes = NULL;
	char          user[] = "root";
	char          passwd[] = "0000000027";
	char          db[] = "cpp";
	pt_Mysql      pm;
	gqrm_id_t     src = 0;
	gqrm_id_t     dsts[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	size_t        n = 10;

	srand((unsigned)time(0));

	if (argc != 2)
	    exit(-1);
	size = atoi(argv[1]);

// create, init
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

	if ((spt = ALGraph_ShortestPathTree(pg, src, dsts, n)) == NULL) {
	    printf("fail to build shortest path tree\n");
	} else {
    	printf("Dijkstra shortest path tree:\n");
        ALGraph_Print(spt, stdout);
	}

/* copy
    printf("copy test:\n");
	if ((cpy = ALGraph_Copy(pg)) == NULL)
	    printf("copy fail\n");
	else
	    ALGraph_Print(pg, stdout);
*/
    
	if ((pm = Mysql_CreateInitConnect(user, passwd, db)) == NULL)
	    exit(-1);
	Mysql_Query(pm, "DELETE FROM graph;");
	Mysql_Query(pm, "DELETE FROM graph1;");
    if (Mysql_WriteGraph(pg, pm, "graph") == DS_ERROR)
	    printf("pg mysql fails\n");
	else
	    printf("pg mysql done\n");

    if (Mysql_WriteGraph(spt, pm, "graph1") == DS_ERROR)
	    printf("spt mysql fails\n");
	else
	    printf("spt mysql done\n");

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
