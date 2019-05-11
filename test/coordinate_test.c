
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
#include "../src/coordinate.h"
#include "../src/random.h"

int main(int argc, char* argv[])
{
    pt_Coordinate co, co1, res;
    int i;
    coordinate_t x, y;

    srand((unsigned)time(0));

    Coordinate_Create2D(0.0, 0.0);

    for (i = 0; i < 400; i++) {
        co = Coordinate_CreateRandom2D(100, -100);
        co1 = Coordinate_CreateRandom2D(100, -100);
        printf("coordinate 1:\n");
        Coordinate_2DPrint(co, stdout);
        Coordinate_GetX(co, &x);
        Coordinate_GetY(co, &y);
        printf("x-> %lf, y-> %lf", x ,y);
        printf("\n");
        printf("coordinate 2:\n");
        Coordinate_2DPrint(co1, stdout);
        if (Coordinate_Equal(co, co1) == DS_TRUE)
            printf("equal, ");
        else
            printf("not equal, ");
        if (Coordinate_Equal(co, co) == DS_TRUE)
            printf("self equal\n");
        else
            printf("self not equal\n");
        printf("their distance is %lf\n", Coordinate_Distance(co, co1));
        Coordinate_Add(co, co1, res);
        printf("addition: "); Coordinate_2DPrint(res, stdout);
        Coordinate_Subtract(co, co1, res);
        printf("subtraction: "); Coordinate_2DPrint(res, stdout);
        printf("\n");
        Coordinate_Free(&co);
    }
    return 0;
}
