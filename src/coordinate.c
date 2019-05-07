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
#include <math.h>
#include <assert.h>

#include "header.h"
#include "coordinate.h"

struct COORDINATE {
    coordinate_t    x;
    coordinate_t    y;
    coordinate_t    z;
};

typedef enum {
    x_field, y_field, z_field
} coor_field;

static ds_stat get_coordinate(pt_Coordinate, const coor_field, coordinate_t*);
static ds_stat set_coordinate(pt_Coordinate, const coor_field, const coordinate_t);
static ds_bool check_equality(pt_Coordinate, pt_Coordinate, coor_field);

/* @fn
 * Create a 2D coordinate with given x-coordinate and
 * y-coordinate, and initialize the z-coordinate to 0.0.
 */
pt_Coordinate
Coordinate_Create2D(const coordinate_t x, const coordinate_t y)
{
    pt_Coordinate    co = malloc(sizeof(Coordinate));

    if (!co)
        return NULL;
    assert(co);

    co->x = x;
    co->y = y;
    co->z = 0.0;

    return co;
}

/* @fn
 * Create a 3D coordinate with given parameters.
 */
pt_Coordinate
Coordinate_Create3D(const coordinate_t x, const coordinate_t y, 
                    const coordinate_t z)
{
    pt_Coordinate co = malloc(sizeof(Coordinate));

    if (!co)
        return NULL;
    assert(co);

    co->x = x;
    co->y = y;
    co->z = z;

    return co;
}

/* @fn
 * Get the x-coordinate of a coorinate, returning through 
 * parameter "x".
 */
ds_stat
Coordinate_GetX(pt_Coordinate co, coordinate_t* x)
{
    return get_coordinate(co, x_field, x);
}

/* @fn
 * Get the y-coordinate of a coorinate, returning through 
 * parameter "y".
 */
ds_stat
Coordinate_GetY(pt_Coordinate co, coordinate_t* y)
{
    return get_coordinate(co, y_field, y);
}

/* @fn
 * Get the z-coordinate of a coorinate, returning through 
 * parameter "z".
 */
ds_stat
Coordinate_GetZ(pt_Coordinate co, coordinate_t* z)
{
    return get_coordinate(co, z_field, z);
}

/* @fn
 * Get x-, y-, or z-coordinate according to parameters.
 */
static ds_stat
get_coordinate(pt_Coordinate co, const coor_field f, coordinate_t* re)
{
    if (!co || !re)
        return DS_ERROR;

    switch (f) {
        case x_field:
            *re = co->x; break;
        case y_field:
            *re = co->y; break;
        case z_field:
            *re = co->z; break;
        default:
            return DS_ERROR;
    }
    return DS_OK;
}

/* @fn
 * Set the x-coordinate of "co" to "x".
 */
ds_stat
Coordinate_SetX(pt_Coordinate co, const coordinate_t x)
{
    return set_coordinate(co, x_field, x);
}

/* @fn
 * Set the y-coordinate of "co" to "y".
 */
ds_stat
Coordinate_SetY(pt_Coordinate co, const coordinate_t y)
{
    return set_coordinate(co, y_field, y);
}

/* @fn
 * Set the z-coordinate of "co" to "z".
 */
ds_stat
Coordinate_SetZ(pt_Coordinate co, const coordinate_t z)
{
    return set_coordinate(co, z_field, z);
}

/* @fn
 * Set x-, y-, or z-coordinate according to parameters.
 */
static ds_stat
set_coordinate(pt_Coordinate co, const coor_field f, const coordinate_t o)
{
    if (!co)
        return DS_ERROR;

    switch (f) {
        case x_field:
            co->x = o; break;
        case y_field:
            co->y = o; break;
        case z_field:
            co->z = o; break;
        default:
            return DS_ERROR;
    }
    return DS_OK;
}

/* @fn
 * lhs = rhs
 */
ds_stat
Coordinate_Assign(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (!lhs || !rhs)
        return DS_ERROR;

    lhs->x = rhs->x;
    lhs->y = rhs->y;
    lhs->z = rhs->z;

    return DS_OK;
}

/* @fn
 * Check whether two coordinates are identical. Be aware,
 * it's the responsibility of the caller to ensure the
 * given pointers to coordinate are not null. IF EITHER 
 * NULL, PROGRAM WILL HALT.
 */
ds_bool
Coordinate_Equal(pt_Coordinate lhs, pt_Coordinate rhs)
{
    assert(lhs);
    assert(rhs);

    if (
        Coordinate_XEqual(lhs, rhs) == DS_TRUE &&
        Coordinate_YEqual(lhs, rhs) == DS_TRUE &&
        Coordinate_ZEqual(lhs, rhs) == DS_TRUE
       )
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn 
 * Check whether two coordinates are different.
 */
ds_bool
Coordinate_NotEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (Coordinate_Equal(lhs, rhs) == DS_FALSE)
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn 
 * Check whether the x-coordinates of two coordinates 
 * are equal.
 */
ds_bool
Coordinate_XEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    return check_equality(lhs, rhs, x_field);
}

ds_bool
Coordinate_XNotEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (Coordinate_XEqual(lhs, rhs) == DS_FALSE)
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn 
 * Check whether the y-coordinates of two coordinates 
 * are equal.
 */
ds_bool
Coordinate_YEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    return check_equality(lhs, rhs, y_field);
}

ds_bool
Coordinate_YNotEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (Coordinate_YEqual(lhs, rhs) == DS_FALSE)
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn 
 * Check whether the z-coordinates of two coordinates 
 * are equal.
 */
ds_bool
Coordinate_ZEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    return check_equality(lhs, rhs, z_field);
}

ds_bool
Coordinate_ZNotEqual(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (Coordinate_ZEqual(lhs, rhs) == DS_FALSE)
        return DS_TRUE;
    return DS_FALSE;
}

static ds_bool
check_equality(pt_Coordinate lhs, pt_Coordinate rhs, coor_field f)
{
    assert(lhs);
    assert(rhs);

    switch (f) {
        case x_field:
            if (lhs->x == rhs->x)
                return DS_TRUE;
            return DS_FALSE;
        case y_field:
            if (lhs->y == rhs->y)
                return DS_TRUE;
            return DS_FALSE;
        case z_field:
            if (lhs->z == rhs->z)
                return DS_TRUE;
            return DS_FALSE;
        default:
            return DS_FALSE;
    }
}

/* @fn
 * Perform addition of two coordinates, and return result through
 * parameter "res".
 */
ds_stat
Coordinate_Add(pt_Coordinate lhs, pt_Coordinate rhs, pt_Coordinate res)
{
    if (!lhs || !rhs || !res)
        return DS_ERROR;
    
    res->x = lhs->x - rhs->x;
    res->y = lhs->y - rhs->y;
    res->z = lhs->z - rhs->z;

    return DS_OK;
}

/* @fn
 * Subtract "rhs" from "lhs" (i.e., lhs - rhs), and return result through
 * parameter "res".
 */
ds_stat
Coordinate_Subtract(pt_Coordinate lhs, pt_Coordinate rhs, pt_Coordinate res)
{
    if (!lhs || !rhs || !res)
        return DS_ERROR;
    
    res->x = lhs->x - rhs->x;
    res->y = lhs->y - rhs->y;
    res->z = lhs->z - rhs->z;

    return DS_OK;
}

/* @fn
 * Perform addition, and result is stored on "lhs" (i.e., lhs += rhs).
 */
ds_stat
Coordinate_OnbodyAdd(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (!lhs || !rhs)
        return DS_ERROR;

    lhs->x += rhs->x;
    lhs->y += rhs->y;
    lhs->z += rhs->z;

    return DS_OK;
}

/* @fn
 * lhs -= rhs.
 */
ds_stat
Coordinate_OnbodySubtract(pt_Coordinate lhs, pt_Coordinate rhs)
{
    if (!lhs || !rhs)
        return DS_ERROR;

    lhs->x -= rhs->x;
    lhs->y -= rhs->y;
    lhs->z -= rhs->z;
}

/* @fn
 * Compute the Euclidean distance between two coordinates.
 */
coordinate_t
Coordinate_Distance(pt_Coordinate c1, pt_Coordinate c2)
{
    coordinate_t  x, y, z;

    assert(c1);
    assert(c2);

    x = fabs(c1->x - c2->x);
    y = fabs(c1->y - c2->y);
    z = fabs(c1->z - c2->z);

    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

ds_stat
Coordinate_2DPrint(pt_Coordinate co, FILE* fp)
{
    if (!co || !fp)
        return DS_ERROR;

    fprintf(fp, "(%lf, %lf)", co->x, co->y);

    return DS_OK;
}

ds_stat
Coordinate_3DPrint(pt_Coordinate co, FILE* fp)
{
    if (!co || !fp)
        return DS_ERROR;

    fprintf(fp, "(%lf, %lf, %lf)", co->x, co->y, co->z);

    return DS_OK;
}
