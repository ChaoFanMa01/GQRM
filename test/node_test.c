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

int main(int argc, char* argv[])
{
    pt_Node   nd1, nd2;
    int       size;
    int       i, type, id = 0;

    srand((unsigned)time(0));

    if (argc != 2) {
        printf("parameter error!\n");
        exit(-1);
    }

    size = atoi(argv[1]);

    /* create, free
    for (i = 0; i < size; i++) {
        type = rand() % 3;
        switch (type) {
            case 0: nd1 = Node_CreateRandomSN(id++, 10.0, 10); break;
            case 1: nd1 = Node_CreateRandomCDL(id++, 10.0, 10); break;
            case 2: nd1 = Node_CreateRandomGW(id++, 10.0, 10); break;
        }
        Node_2DPrint(nd1, stdout); printf("\n");
        Node_Free(&nd1);
    }
    */

    for (i = 0; i < size; i++) {
        nd1 = Node_CreateRandomSN(id++, 10.0, 10);
        nd2 = Node_CreateRandomCDL(id++, 10.0, 10);
        Node_2DPrint(nd1, stdout); printf("\n");
        Node_2DPrint(nd2, stdout); printf("\n");
        printf("Distance: %lf", Node_Distance(nd1, nd2));
        if (Node_IsNeighbor(nd1, nd2) == DS_TRUE)
            printf(" neighbor\n");
        else
            printf(" not neighbor\n");
    }

    return 0;
}
